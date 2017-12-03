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

bool Player::CreateFromWorld(World & world, const Vector3 &Pos)
{
   EventChannel channel;
   channel.Add<InputManager::KeyDownEvent, Player>(*this);
   channel.Add<InputManager::KeyReleasedEvent, Player>(*this);
   channel.Add<InputManager::KeyPressedEvent, Player>(*this);
   channel.Add<InputManager::MousePressedEvent, Player>(*this);
   _entity = world.CreateEntity();

   TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
   transComp._position = Vector3(0, TERRAIN->GetHeight(0.0f, 0.0f), 0);
   _pTransformComp = &transComp;
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
   collision._type = CollisionComponent::COLLISION_TYPE_OBB;

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
   _attackToStanceTimer.Reset(0.4f);

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
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				}
			}
			else
			{
				_state = PLAYERSTATE_MOVE;
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
				}
				else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
				}
				else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
				}
			}
         }
         else if (_currentCommand._type == GAMECOMMAND_ACTION)
         {
            if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
            {
               _inCombat = true;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
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
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
            break;;
         }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _state = PLAYERSTATE_MOVE;
            _combatToPeaceTimer.Restart();
            
            if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
            {
				_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
            }
            else if(_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
				_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
            }
            else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
            {
				_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
            }
            else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
				_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
            }
         }
         else if (_currentCommand._type == GAMECOMMAND_ACTION)
         {
            if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
            {
               _state = PLAYERSTATE_ATTACK;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
            }
            else if (_currentCommand._behavior._type == BEHAVIOR_BLOCK)
            {
               _state = PLAYERSTATE_BLOCK;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarShieldBlock));
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
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
            break;;
         }

		 if (_currentCommand._dash &&
			 _currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		 {
			 _state = PLAYERSTATE_RUN;
			 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
			 break;
		 }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();
            if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
            {
               if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
				   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
                  this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
			   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			   {
				   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
				   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
                  this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
			   }
			   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			   {
				   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
				   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
                  this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
			   }
            }
            else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
				}
			}
         }
		 else if (_currentCommand._type == GAMECOMMAND_ACTION)
		 {
			 if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
			 {
				 _state = PLAYERSTATE_MOVEATTACK;
				 _inCombat = true;
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingLeft));
				 break;
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
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			break;
         }

		 if (_currentCommand._dash &&
			 _currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		 {
			 _state = PLAYERSTATE_RUN;
			 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
			 break;
		 }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();

			if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
			{
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
				}
				else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
				   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
				}
				else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
				   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
				}
			}
			else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
			{
				if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				}
			}

			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
				}
				else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
				{
					_currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
				}
			}
         }
		 else if (_currentCommand._type == GAMECOMMAND_ACTION)
		 {
			 if (_currentCommand._behavior._type == BEHAVIOR_ATTACK && 
				 _currentMovement._vertical != VERTICAL_MOVEMENT_DOWN)
			 {
				 _state = PLAYERSTATE_MOVEATTACK;
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingLeft));
				 break;
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
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
			   }
		   }
		   else
		   {
			   _state = PLAYERSTATE_MOVE;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
	   }
   }break;

   case Player::PLAYERSTATE_ATTACK:
   {
      if (_inCombat)
      {
         if (_canCombo && 
			 _attackToStanceTimer.Tick(deltaTime))
         {
            _state = PLAYERSTATE_STANCE;
			MovementStop(_currentMovement);
            _attackToStanceTimer.Restart();
            _comboCount = 0;
			_canCombo = false;
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			break;
         }

         if (_currentCommand._behavior._type == BEHAVIOR_ATTACK && _canCombo)
         {
               _attackToStanceTimer.Restart();
			   _canCombo = false;
			  
			   if (_comboCount == 0)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingRight));
			   }
			   else if (_comboCount == 1)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarThrustMid));
				   _comboCount = 0;
				   break;
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
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
            break;;
         }
      }
   } break;
  
   case Player::PLAYERSTATE_MOVEATTACK :
   {
	   if (_inCombat)
	   {
		   //bool toStance = _attackToStanceTimer.Tick(deltaTime);
		   if (_canCombo && _attackToStanceTimer.Tick(deltaTime))
		   {
			   if (IsMovementNone(_currentCommand._movement))
			   {
				   _state = PLAYERSTATE_STANCE;
				   _canCombo = false;
				   MovementStop(_currentMovement);
				   _attackToStanceTimer.Restart();
				   _comboCount = 0;
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
				   break;
			   }
			   else
			   {
				   _state = PLAYERSTATE_MOVE;
				   _attackToStanceTimer.Restart();
				   _comboCount = 0;
				   if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				   {
					   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
					   {
						   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
					   }
					   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
					   {
						   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
					   }
					   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
					   {
						   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
					   }
				   }
				   else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				   {
					   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
					   {
						   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
					   }
					   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
					   {
						   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
					   }
					   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
					   {
						   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
					   }
				   }
				   else if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
				   {
					   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
					   {
						   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
					   }
					   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
					   {
						   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
					   }
					   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
					   {
						   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
						   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
					   }
				   }
				   break;
			   }
		   }

		   if (_currentCommand._behavior._type == BEHAVIOR_ATTACK && _canCombo)
		   {
			   _attackToStanceTimer.Restart();
			   _canCombo = false;

			   if (_comboCount == 0)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingRight));
			   }
			   else if (_comboCount == 1)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkThrust));
				   _comboCount = 0;
				   break;
			   }
			   _comboCount++;
		   }
	   }
   }break;

   }
   _currentCommand.Reset();

   _channel.Broadcast<PlayerImformationEvent>(
	   PlayerImformationEvent(_pTransformComp->GetWorldPosition(), _state, _pTransformComp->GetForward()));
}

void Player::MoveAndRotate(float deltaTime)
{
	////움직이지 않는 상태일때는 Move
	if (_state == PLAYERSTATE_STANCE ||
		_state == PLAYERSTATE_ATTACK ||
		_state == PLAYERSTATE_BLOCK ||
		(_state == PLAYERSTATE_MOVEATTACK && _currentMovement._vertical == VERTICAL_MOVEMENT_DOWN))
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
   if ('L' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
   }

   if('I' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
   }
   if ('K' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
   }

   if (VK_SHIFT == inputCode)
   {
	   _currentCommand._dash = true;
   }
}

void Player::Handle(const InputManager::KeyPressedEvent & event)
{
   uint32 inputCode = event.code;
   if('J' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
   }
   if ('L' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
   }

   if('I' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
   }
   if ('K' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
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

void Player::QueueAction(const Action & action)
{
   _pActionComp->_actionQueue.PushAction(action);
   _animationEnum = action._enum;
}

