#include "stdafx.h"
#include "Player.h"

#include "PlayerStates.h"

#include "Camera.h"

#define MAX_COMBO_COUNT 3

//#include "stdafx.h"
//#include "Player.h"
//
//#include "PlayerStates.h"
//
//#define MAX_COMBO_COUNT 3

Player::Player()
{
}

Player::~Player()
{
}

bool Player::CreateFromWorld(World & world)
{
   EventChannel channel;
   channel.Add<InputManager::KeyDownEvent, Player>(*this);
   channel.Add<InputManager::KeyReleasedEvent, Player>(*this);
   //channel.Add<InputManager::KeyDownEvent, Player>(*this);
   channel.Add<InputManager::MousePressedEvent, Player>(*this);
   _entity = world.CreateEntity();

   TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
   transComp._position = Vector3(0, TERRAIN->GetHeight(0.0f, 0.0f), 0);
   //transComp.MovePositionWorld();

   static int32 animCount = 0;

   RenderComponent &renderComp = _entity.AddComponent<RenderComponent>();
   renderComp._type = RenderComponent::Type::eSkinned;
   renderComp._skinned = VIDEO->CreateAnimationInstance(VIDEO->GetSkinnedXMesh("Knight"), 
	   "Knight_" + std::to_string(animCount));
   renderComp._arche = ARCHE_HERO;

   video::AnimationInstance *pAnimation = VIDEO->GetAnimationInstance(renderComp._skinned);

   CollisionComponent &collision = _entity.AddComponent<CollisionComponent>();
   collision._boundingBox.Init(pAnimation->_pSkinnedMesh->_boundInfo._min,
      pAnimation->_pSkinnedMesh->_boundInfo._max);
   collision._boundingSphere._localCenter = pAnimation->_pSkinnedMesh->_boundInfo._center;
   collision._boundingSphere._radius = pAnimation->_pSkinnedMesh->_boundInfo._radius;
   collision._triggerType = CollisionComponent::TRIGGER_TYPE_PLAYER;

   ScriptComponent &scriptComponent = _entity.AddComponent<ScriptComponent>();
   scriptComponent.SetScript(MAKE_SCRIPT_DELEGATE(Player, Update, *this));

   _pActionComp = &_entity.AddComponent<ActionComponent>();
   _pActionComp->CreateFrom(renderComp._skinned);
   _pActionComp->_pCallbackHandler = new PlayerCallbackHandler;
   _pActionComp->_pCallbackHandler->Init(this);
   SetupCallbackAndCompression();

   _pActionComp->MakeAnimationList();
   _pActionComp->SetFirstAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));

   _entity.Activate();

   _pStateMachine = new PlayerStateMachine;
   _pStateMachine->Init(this);
   _pStateMachine->RegisterState(META_TYPE(PlayerStanceState)->Name(), new PlayerStanceState());
   _pStateMachine->RegisterState(META_TYPE(PlayerMoveState)->Name(), new PlayerMoveState());
   _pStateMachine->RegisterState(META_TYPE(PlayerCombatState)->Name(), new PlayerCombatState());
   _pStateMachine->RegisterState(META_TYPE(PlayerDeadState)->Name(), new PlayerDeadState());
   _pStateMachine->ChangeState(META_TYPE(PlayerStanceState)->Name());

   //Plyer의 맴버 변수들을 셋팅해주자
   _combatToPeaceTimer.Reset(2.0f);
   _moveToStanceTimer.Reset(0.05f);
   _attackToStanceTimer.Reset(1.5f);

   _attackCombo1Timer.Reset(0.7f);
   _attackCombo2Timer.Reset(0.7f);

   return true;
}

void Player::Update(float deltaTime)
{
   _pStateMachine->Update(deltaTime, _currentCommand);

   MoveAndRotate(deltaTime);

   switch (_state)
   {
   case Player::PLAYERSTATE_STANCE:
   {
      //공격 모드가 아닐때
      if (false == _inCombat)
      {
         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
			if (_currentCommand._dash)
			{
				_state = PLAYERSTATE_RUN;
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				}
			}
			else
			{
				_state = PLAYERSTATE_MOVE;
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
				}
				else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
				}
				else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
				}
			}
         }
         else if (_currentCommand._type == GAMECOMMAND_ACTION)
         {
            if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
            {
               _inCombat = true;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
            }
         }
      }
      //공격 모드 일때
      else 
      {
         bool combatToPeace = _combatToPeaceTimer.Tick(deltaTime);
         if (combatToPeace)
         {
            _inCombat = false;
            _combatToPeaceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
            return;
         }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _state = PLAYERSTATE_MOVE;
            _combatToPeaceTimer.Restart();
            
            if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
            {
				_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
            }
            else if(_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
				_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
            }
            else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
            {
				_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
            }
            else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
				_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
            }
         }
         else if (_currentCommand._type == GAMECOMMAND_ACTION)
         {
            if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
            {
               _state = PLAYERSTATE_ATTACK;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
			   _attackCombo1Timer.Tick(deltaTime);
            }
            else if (_currentCommand._behavior._type == BEHAVIOR_BLOCK)
            {
               _state = PLAYERSTATE_BLOCK;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarShieldBlock));
            }
         }
      }
   } break;
   case Player::PLAYERSTATE_MOVE:
   {
      bool moveToStance = false;
      //인풋이 없을때만 MoveToStanceTimer를 작동시킨다....
      if (IsMovementNone(_currentCommand._movement))
      {
         moveToStance = _moveToStanceTimer.Tick(deltaTime);
      }

      //공격 모드가 아닐때
      if (false == _inCombat)
      {
         //만약 moveToStance 타이머가 울리면 stance상태로 돌아가라
         if (moveToStance)
         {
            _state = PLAYERSTATE_STANCE;
			MovementStop(_currentMovement);
            _moveToStanceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
            return;
         }

		 if (_currentCommand._dash &&
			 _currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		 {
			 _state = PLAYERSTATE_RUN;
			 _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
			 return;
		 }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();
            if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
            {
               if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
            }
            else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
               if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
            }

            if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
				   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
            }
            else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
				   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
            }
         }
		 else if (_currentCommand._type == GAMECOMMAND_ACTION)
		 {
			 if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
			 {
				 _state = PLAYERSTATE_MOVEATTACK;
				 _inCombat = true;
				 _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingLeft));
				 return;
			 }
		 }
      }
      //공격 모드 일때
      else if (true == _inCombat)
      {
         if (moveToStance)
         {
            _state = PLAYERSTATE_STANCE;
			MovementStop(_currentMovement);
            _moveToStanceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
            return;
         }

		 if (_currentCommand._dash &&
			 _currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		 {
			 _state = PLAYERSTATE_RUN;
			 _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
			 return;
		 }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();
			if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
			{
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
			}
			else if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
			{
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					_pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
			}
         }
		 else if (_currentCommand._type == GAMECOMMAND_ACTION)
		 {
			 if (_currentCommand._behavior._type == BEHAVIOR_ATTACK && 
				 _currentMovement._vertical != VERTICAL_MOVEMENT_DOWN)
			 {
				 _state = PLAYERSTATE_MOVEATTACK;
				 _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingLeft));
				 return;
			 }
		 }
      }
   } break;

   case Player::PLAYERSTATE_RUN :
   {
	   if (!_currentCommand._dash )
	   {
		   if (IsMovementNone(_currentCommand._movement))
		   {
			   _state = PLAYERSTATE_STANCE;
			   MovementStop(_currentMovement);
			   if (_inCombat)
			   {
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			   }
			   else
			   {
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
			   }
		   }
		   else
		   {
			   _state = PLAYERSTATE_MOVE;
			   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
		   //if(_currentCommand._movement.)
	   }
   }break;

   case Player::PLAYERSTATE_ATTACK:
   {
      if (_inCombat)
      {
         bool toStance = _attackToStanceTimer.Tick(deltaTime);
         if (toStance)
         {
            _state = PLAYERSTATE_STANCE;
			MovementStop(_currentMovement);
            _attackToStanceTimer.Restart();
            _comboCount = 0;
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			return;
			/*_attackCombo1Timer.Restart();
			_attackCombo2Timer.Restart();*/
         }

         if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
         {
               _attackToStanceTimer.Restart();
			  
			   if (_comboCount == 1/* && _attackCombo1Timer._currentTime > 0.3*/)
			   {
				   _attackCombo2Timer.Tick(deltaTime);
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingRight));
			   }
			   else if (_comboCount == 2 /*&& _attackCombo2Timer._currentTime > 0.3*/)
			   {
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarThrustMid));
				   _comboCount = 0;
				   return;
			   }			   
            _comboCount++;
         }
      }
   } break;

   case Player::PLAYERSTATE_BLOCK:
   {
      if (_inCombat)
      {
         bool toStance = _attackToStanceTimer.Tick(deltaTime);
         if (toStance)
         {
            _state = PLAYERSTATE_STANCE;
			MovementStop(_currentMovement);
            _attackToStanceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
            return;
         }
      }
   } break;
  
   case Player::PLAYERSTATE_MOVEATTACK :
   {
	   if (_inCombat)
	   {
		   bool toStance = _attackToStanceTimer.Tick(deltaTime);
		   if (toStance)
		   {
			   if (IsMovementNone(_currentMovement))
			   {
				   _state = PLAYERSTATE_STANCE;
				   MovementStop(_currentMovement);
				   _attackToStanceTimer.Restart();
				   _comboCount = 0;
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
				   return;
			   }
			   else
			   {
				   _state = PLAYERSTATE_MOVE;
				   _attackToStanceTimer.Restart();
				   _comboCount = 0;
				   if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				   {
					   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
				   }
				   else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				   {
					   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
				   }
				   else if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
				   {
					   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				   }
				   return;
			   }
		   }

		   if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
		   {
			   _attackToStanceTimer.Restart();

			   if (_comboCount == 1/* && _attackCombo1Timer._currentTime > 0.3*/)
			   {
				   _attackCombo2Timer.Tick(deltaTime);
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingRight));
			   }
			   else if (_comboCount == 2 /*&& _attackCombo2Timer._currentTime > 0.3*/)
			   {
				   _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkThrust));
				   _comboCount = 0;
				   return;
			   }
			   _comboCount++;
		   }
	   }
   }break;

   }

   _currentCommand.Reset();
}

void Player::SetupCallbackAndCompression()
{
   ActionComponent &refActionComp = _entity.GetComponent<ActionComponent>();
   TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

   ID3DXAnimationController *pController = refActionComp._pAnimationController;
   uint32 numAnimationSet = pController->GetNumAnimationSets();
   ID3DXKeyframedAnimationSet *anim0;

   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft], (ID3DXAnimationSet **)&anim0);

   _callbackData._animtionEnum = (PlayerAnimationEnum *)&_animationEnum;

   D3DXKEY_CALLBACK warSwingLeftKeys;
   warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
   warSwingLeftKeys.pCallbackData = (void *)&_callbackData;

   AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Player::MoveAndRotate(float deltaTime)
{
	////움직이지 않는 상태일때는 Move
	if (_state == PLAYERSTATE_STANCE ||
		_state == PLAYERSTATE_ATTACK ||
		_state == PLAYERSTATE_BLOCK)
	{
		return;
	}

	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	Vector3 forward = refTransform.GetForward();
	Vector3 right = refTransform.GetRight();

	Vector3 toMove;

	if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
	{
		toMove -= right;
	}
	else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
	{
		toMove += right;
	}

	if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
	{
		toMove += forward;
	}
	else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
	{
		toMove -= forward;
	}

   if (!toMove.IsZero())
   {
	   Vec3Normalize(&toMove, &toMove);

	   toMove *= deltaTime;

	   switch (_state)
	   {
	   //case Player::PLAYERSTATE_STANCE:
	   //{
	   //} break;
	   case Player::PLAYERSTATE_MOVE:
	   {
		   refTransform.MovePositionWorld(toMove * _walkSpeed);
	   } break;
	   case Player::PLAYERSTATE_RUN:
	   {
		   refTransform.MovePositionWorld(toMove * _runSpeed);
	   } break;
	   case Player::PLAYERSTATE_ATTACK:
	   {
	   } break;
	   case Player::PLAYERSTATE_BLOCK:
	   {
	   } break;
	   case Player::PLAYERSTATE_MOVEATTACK:
	   {
		   refTransform.MovePositionWorld(toMove * _walkSpeed * 0.7f);
	   } break;
	   }

      refTransform.SetWorldPosition(
		  refTransform.GetWorldPosition().x, 
		  TERRAIN->GetHeight(refTransform.GetWorldPosition().x, refTransform.GetWorldPosition().z), refTransform.GetWorldPosition().z);
   }
}

void Player::Handle(const InputManager::KeyDownEvent & event)
{
   uint32 inputCode = event.code;
   if('J' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
   }
   else if ('L' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
   }
   if('I' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
   }
   else if ('K' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
   }
   if (VK_SHIFT == inputCode)
   {
	   _currentCommand._dash = true;
   }
}

void Player::Handle(const InputManager::KeyReleasedEvent & event)
{
   uint32 inputCode = event.code;
   if('J' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_NONE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
   }
   else if ('L' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_NONE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
   }
   if('I' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_NONE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
   }
   else if ('K' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_NONE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
   }
   if (VK_SHIFT == inputCode)
   {
	   _currentCommand._dash = false;
   }
}

void Player::Handle(const InputManager::MousePressedEvent & event)
{
   uint32 inputCode = event.code;

   if (inputCode == MOUSE_BUTTON_LEFT)
   {
      _currentCommand._type = GAMECOMMAND_ACTION;
      _currentCommand._behavior._type = BEHAVIOR_ATTACK;
   }
   else if (inputCode == MOUSE_BUTTON_RIGHT)
   {
      _currentCommand._type = GAMECOMMAND_ACTION;
      _currentCommand._behavior._type = BEHAVIOR_BLOCK;
   }
}

//void Player::Handle(const InputManager::KeyDownEvent & event)
//{
//   uint32 inputCode = event.code;
//
//   TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
//
//   Vector3 Dir = _camera->GetEntity().GetComponent<TransformComponent>().GetForward();
//
//   Dir.y = 0;
//
//   D3DXVec3Normalize(&Dir, &Dir);
//
//   if ('L' == inputCode)
//   {
//	   //transComp.SetForward(-Dir);
//	  // transComp.LookDirection(Dir);
//      _currentCommand._type = GAMECOMMAND_MOVE;
//      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
//      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
//   }
//   else if('J' == inputCode)
//   {
//	   //transComp.SetForward(-Dir);
//	  // transComp.LookDirection(Dir);
//      _currentCommand._type = GAMECOMMAND_MOVE;
//      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
//      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
//   }
//   else if('I' == inputCode)
//   {
//	   //transComp.SetForward(-Dir);
//	   //transComp.LookDirection(Dir);
//      _currentCommand._type = GAMECOMMAND_MOVE;
//      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
//      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
//   }
//   else if ('K' == inputCode)
//   {
//	   //transComp.SetForward(-Dir);
//	  // transComp.LookDirection(Dir);
//      _currentCommand._type = GAMECOMMAND_MOVE;
//      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
//      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
//   }
//   else if (VK_SPACE == inputCode)
//   {
//      _currentCommand._type = GAMECOMMAND_JUMP;
//   }
//   /*else
//   {
//      _currentCommand._type = GAMECOMMAND_NONE;
//      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
//      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
//   }*/
//}

void Player::QueueAction(const Action & action)
{
   _pActionComp->_actionQueue.PushAction(action);
}

HRESULT PlayerCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	PlayerCallbackData* pData = (PlayerCallbackData*)pCallbackData;
	if (nullptr == pData)
	{
		return S_OK;
	}
	switch (*pData->_animtionEnum)
	{
	case PlayerAnimationEnum::eWarSwingLeft:
	{
		int a = 0;
	}break;

	}
	return S_OK;
}