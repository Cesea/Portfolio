#include "stdafx.h"
#include "Player.h"

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

   //channel.Add<CollisionSystem::ActorTriggerEvent, Player>(*this);

   _entity = world.CreateEntity();

   TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
   transComp._position = Vector3(0, TERRAIN->GetHeight(0.0f, 0.0f), 0);
   _pTransformComp = &transComp;

   TERRAIN->ConvertWorldPostoTilePos(transComp.GetWorldPosition(), &_tilePos);

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
   collision._type = CollisionComponent::COLLISION_TYPE_BOX;
   _pCollisionComp = &collision;


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

   _channel.Broadcast<GameObjectFactory::ObjectCreatedEvent>(
	   GameObjectFactory::ObjectCreatedEvent(ARCHE_HERO, _entity, transComp.GetWorldPosition()));

   //Plyer의 맴버 변수들을 셋팅해주자
   _combatToPeaceTimer.Reset(2.0f);
   _moveToStanceTimer.Reset(0.15f);
   _attackToStanceTimer.Reset(0.4f);
   _attackTriggerTimer.Reset(0.08f);

   return true;
}

void Player::Update(float deltaTime)
{
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
			 _state = PLAYERSTATE_MOVE;
			 _pActionComp->_actionQueue.ClearQueue();
			 if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
			 {
				 if (_currentCommand._dash)
				 {
					 _state = PLAYERSTATE_RUN;
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 }
				 else
				 {
					 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				 }
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
            
            if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
            {
				_pActionComp->_actionQueue.ClearQueue();
				 if (_currentCommand._dash)
				 {
					 _state = PLAYERSTATE_RUN;
					_currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 }
				 else
				 {
					 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
					 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
				 }
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
            else if(_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
				_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
               this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
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
			_pActionComp->_actionQueue.ClearQueue();
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
            break;
         }

		 if (_currentCommand._dash)
		 {
			 if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
			 {
				 _state = PLAYERSTATE_RUN;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
			 }
			 else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
			 {
				 _state = PLAYERSTATE_RUN;
				 _pActionComp->_actionQueue.ClearQueue();
				 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 _camRotated = true;
				 _targetRotation = D3DX_PI;
			 }
			 else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			 {
				 _state = PLAYERSTATE_RUN;
				 _currentMovement._horizontal = HORIZONTAL_MOVEMENT_NONE;
				 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 _camRotated = true;
				 _targetRotation = PI_DIV_2;
			 }
			 else
			 {
				 _state = PLAYERSTATE_RUN;
				 _currentMovement._horizontal = HORIZONTAL_MOVEMENT_NONE;
				 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 _camRotated = true;
				 _targetRotation = -PI_DIV_2;
			 }
			 break;
		 }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();
			if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
				}
			}

         }
		 else if (_currentCommand._type == GAMECOMMAND_ACTION)
		 {
			 if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
			 {
				 _state = PLAYERSTATE_MOVEATTACK;
				 _inCombat = true;
				 _pActionComp->_actionQueue.ClearQueue();
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
			_pActionComp->_actionQueue.ClearQueue();
            this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			break;
         }

		 if (_currentCommand._dash)
		 {
			 if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
			 {
				 _state = PLAYERSTATE_RUN;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
			 }
			 else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
			 {
				 _state = PLAYERSTATE_RUN;
				 _pActionComp->_actionQueue.ClearQueue();
				 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 _camRotated = true;
				 _targetRotation = D3DX_PI;
			 }
			 else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			 {
				 _state = PLAYERSTATE_RUN;
				 _currentMovement._horizontal = HORIZONTAL_MOVEMENT_NONE;
				 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 _camRotated = true;
				 _targetRotation = PI_DIV_2;
			 }
			 else
			 {
				 _state = PLAYERSTATE_RUN;
				 _currentMovement._horizontal = HORIZONTAL_MOVEMENT_NONE;
				 _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eRun));
				 _camRotated = true;
				 _targetRotation = -PI_DIV_2;
			 }
			 break;
		 }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();

			if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
				}
			}
			else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			{
				if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				{
					_currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
				}
			}
         }
		 else if (_currentCommand._type == GAMECOMMAND_ACTION)
		 {
			 if (_currentCommand._behavior._type == BEHAVIOR_ATTACK && 
				 _currentMovement._vertical != VERTICAL_MOVEMENT_DOWN)
			 {
				 _state = PLAYERSTATE_MOVEATTACK;
				 _pActionComp->_actionQueue.ClearQueue();
				 this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingLeft));
				 break;
			 }
		 }
      }
   } break;

   case Player::PLAYERSTATE_RUN :
   {
	   if (IsMovementNone(_currentCommand._movement))
	   {
		   _state = PLAYERSTATE_STANCE;
		   MovementStop(_currentMovement);
		   _pActionComp->_actionQueue.ClearQueue();
		   if (_inCombat)
		   {
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
		   }
		   else
		   {
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
		   }
		   break;
	   }

	   if (!_currentCommand._dash)
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _state = PLAYERSTATE_MOVE;
			   _pActionComp->_actionQueue.ClearQueue();
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
		   else
		   {
			   _state = PLAYERSTATE_STANCE;
			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
			   }
		   }
	   }
   }break;

   case Player::PLAYERSTATE_ATTACK:
   {
      if (_inCombat)
      {
		  if (_canCombo && _attackTriggerTimer.Tick(deltaTime))
		  {
			  _attackTriggerTimer.Restart();
			  _pCollisionComp->_isTrigger = false;
		  }
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
		  if (_canCombo && _attackToStanceTimer.Tick(deltaTime))
		  {
			  _state = PLAYERSTATE_STANCE;
			  MovementStop(_currentMovement);
			  _attackToStanceTimer.Restart();
			  _pActionComp->_actionQueue.ClearQueue();
			  this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			  break;
		  }
	  }
   } break;
  
   case Player::PLAYERSTATE_MOVEATTACK :
   {
	   if (_inCombat)
	   {
		  if (_canCombo && _attackTriggerTimer.Tick(deltaTime))
		  {
			  _attackTriggerTimer.Restart();
			  _pCollisionComp->_isTrigger = false;
		  }

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
				   _pActionComp->_actionQueue.ClearQueue();
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
				   break;
			   }
			   else
			   {
				   _state = PLAYERSTATE_MOVE;
				   _attackToStanceTimer.Restart();
				   _comboCount = 0;
				   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
				   {
					   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
					   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
				   }
				   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
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

   case Player::PLAYERSTATE_HURT :
   {

   }break;

   case Player::PLAYERSTATE_DEAD :
   {

   }break;
   }
   _prevCommand = _currentCommand;
   _currentCommand.Reset();

   _channel.Broadcast<PlayerImformationEvent>(
	   PlayerImformationEvent(_pTransformComp->GetWorldPosition(), _state, _pTransformComp->GetForward()));
}

void Player::MoveAndRotate(float deltaTime)
{
	float absMinus = absFloat(_targetRotation) - absFloat(_currentRotation);

	if (!FloatZero(absMinus * 0.1f))
	{
		if (_targetRotation < 0.0f)
		{
			_pTransformComp->LookDirection(-_pTransformComp->GetRight(), -_rotationSpeed * deltaTime);
			_currentRotation -= _rotationSpeed * deltaTime;
		}
		else if (_targetRotation > 0.0f)
		{
			_pTransformComp->LookDirection(-_pTransformComp->GetRight(), _rotationSpeed * deltaTime);
			_currentRotation += _rotationSpeed * deltaTime;
		}
		else if (_targetRotation == 0.0f)
		{
			if (_currentRotation < 0.0f)
			{
				_pTransformComp->LookDirection(-_pTransformComp->GetRight(), _rotationSpeed * deltaTime);
				_currentRotation += _rotationSpeed * deltaTime;
			}
			else if (_currentRotation > 0.0f)
			{
				_pTransformComp->LookDirection(-_pTransformComp->GetRight(), -_rotationSpeed * deltaTime);
				_currentRotation -= _rotationSpeed * deltaTime;
			}
		}
	}
	else
	{
		if (_camRotated)
		{
			_targetRotation = 0.0f;
			_currentRotation = 0.0f;
			_camRotated = false;
		}
		else
		{
			_currentRotation = _targetRotation;
		}
	}

	////움직이지 않는 상태일때는 Move
	if (_state == PLAYERSTATE_STANCE ||
		_state == PLAYERSTATE_ATTACK ||
		_state == PLAYERSTATE_BLOCK ||
		_state == PLAYERSTATE_DEAD ||
		_state == PLAYERSTATE_HURT ||
		(_state == PLAYERSTATE_MOVEATTACK && _currentMovement._vertical == VERTICAL_MOVEMENT_DOWN))
	{
		return;
	}

	TransformComponent &refTransform = _entity.GetComponent<TransformComponent>();

	Vector3 forward = refTransform.GetForward();
	Vector3 right = refTransform.GetRight();

	Vector3 toMove;

	bool rightAdded{ false };
	bool forwardAdded{ false };

	if (_animationEnum == PlayerAnimationEnum::eStrafeLeft ||
		_animationEnum == PlayerAnimationEnum::eWarMovingLeft)
	{
		toMove -= right * _walkSpeed;
	}
	else if (_animationEnum == PlayerAnimationEnum::eStrafeRight ||
		_animationEnum == PlayerAnimationEnum::eWarMovingRight)
	{
		toMove += right * _walkSpeed;;
	}
	else
	{
		if (_animationEnum == PlayerAnimationEnum::eWarCharging)
		{
			if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
			{
				toMove += forward * _runSpeed;
			}
			else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
			{
				toMove -= forward * _runSpeed;
			}
		}
		else
		{
			if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
			{
				toMove += forward * _walkSpeed;
			}
			else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
			{
				toMove -= forward * _walkSpeed;
			}
		}
	}

   if (!toMove.IsZero())
   {
	   Vec3Normalize(&toMove, &toMove);

	   toMove *= deltaTime;

	   switch (_state)
	   {
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

	  _prevTilePos = _tilePos;
	  TERRAIN->ConvertWorldPostoTilePos(refTransform.GetWorldPosition(), &_tilePos);
	  RepositionEntity(_tilePos, _prevTilePos);
   }
}

void Player::Handle(const InputManager::KeyDownEvent & event)
{
   uint32 inputCode = event.code;

   if('J' == inputCode)
   {
	   if (_currentCommand._movement._horizontal != HORIZONTAL_MOVEMENT_NONE)
	   {
		   return;
	   }
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
   }

   if ('L' == inputCode)
   {
	   if (_currentCommand._movement._horizontal != HORIZONTAL_MOVEMENT_NONE)
	   {
		   return;
	   }
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
   }

   if('I' == inputCode)
   {
	   if (_currentCommand._movement._vertical != VERTICAL_MOVEMENT_NONE)
	   {
		   return;
	   }
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
   }

   if ('K' == inputCode)
   {
	   if (_currentCommand._movement._vertical != VERTICAL_MOVEMENT_NONE)
	   {
		   return;
	   }
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

   switch (_state)
   {
   case Player::PLAYERSTATE_STANCE:
	   break;
   case Player::PLAYERSTATE_MOVE:
   {
	   if (_animationEnum == PlayerAnimationEnum::eWalk)
	   {
		   if (inputCode == 'J')
		   {
			   _targetRotation = PI_DIV_4;
		   }
		   else if (inputCode == 'L')
		   {
			   _targetRotation = -PI_DIV_4;
		   }
	   }
	   else if(_animationEnum == PlayerAnimationEnum::eWalkingBack ||
		   _animationEnum == PlayerAnimationEnum::eWarRetreat)
	   {
		   if (inputCode == 'J')
		   {
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (inputCode == 'L')
		   {
			   _targetRotation = PI_DIV_4;
		   }
	   }
	   else if (_animationEnum == PlayerAnimationEnum::eStrafeLeft || 
		   _animationEnum == PlayerAnimationEnum::eWarMovingLeft)
	   {
		   if (inputCode == 'I')
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));

			   _targetRotation = PI_DIV_4;
		   }
		   else if (inputCode == 'K')
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
			   }
			   _targetRotation = -PI_DIV_4;
		   }
	   }
	   else if (_animationEnum == PlayerAnimationEnum::eStrafeRight ||
		   _animationEnum == PlayerAnimationEnum::eWarMovingRight)
	   {
		   if (inputCode == 'I')
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (inputCode == 'K')
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
			   }
			   _targetRotation = PI_DIV_4;
		   }
	   }

   } break;
   case Player::PLAYERSTATE_RUN:
   {
	   if (inputCode == 'J')
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
		   else if(_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation = -D3DX_PI;
		   }
		   else
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_2;
		   }
	   }
	   else if (inputCode == 'L')
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
		   else if(_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation = D3DX_PI;
		   }
		   else
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_2;
		   }
	   }
	   else if (inputCode == 'I')
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
	   }
	   else if (inputCode == 'K')
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;

		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
	   }

   } break;
   case Player::PLAYERSTATE_ATTACK:
	   break;
   case Player::PLAYERSTATE_BLOCK:
	   break;
   case Player::PLAYERSTATE_MOVEATTACK:
	   break;
   case Player::PLAYERSTATE_HURT:
	   break;
   case Player::PLAYERSTATE_DEAD:
	   break;
   default:
	   break;
   }

}

void Player::Handle(const InputManager::KeyReleasedEvent & event)
{
   uint32 inputCode = event.code;

   switch (_state)
   {
   case Player::PLAYERSTATE_STANCE:
   {
   } break;
   case Player::PLAYERSTATE_MOVE:
   {
	   if (inputCode == 'I')
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
			   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;

			   _targetRotation = 0.0f;
			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
			   }
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
			   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;

			   _targetRotation = 0.0f;
			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
			   }
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_DOWN;
			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
			   }
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_NONE)
		   {
			   int a = 0;
		   }
	   }
	   else if (inputCode == 'K')
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
			   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
			   _targetRotation = 0.0f;

			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
			   }
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;
			   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;

			   _targetRotation = 0.0f;

			   if (_inCombat)
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
			   }
			   else
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
			   }
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _currentMovement._vertical = VERTICAL_MOVEMENT_UP;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
	   }

	   if (inputCode == 'J')
	   {
		   if (_animationEnum == PlayerAnimationEnum::eWalk)
		   {
			   _targetRotation = 0.0f;
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWalkingBack ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat)
		   {
			   _targetRotation = 0.0f;
		   }

	   }
	   else if (inputCode == 'L')
	   {
		   if (_animationEnum == PlayerAnimationEnum::eWalk)
		   {
			   _targetRotation = 0.0f;
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWalkingBack ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat)
		   {
			   _targetRotation = 0.0f;
		   }
	   }
   } break;
   case Player::PLAYERSTATE_RUN:
   {
	   if (inputCode == 'I')
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
	   }
	   else if (inputCode == 'K')
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
	   }
	   else if (inputCode == 'J')
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
	   }
	   else if (inputCode == 'L')
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
	   }
   } break;
   case Player::PLAYERSTATE_ATTACK:
   {
   } break;
   case Player::PLAYERSTATE_BLOCK:
   {
   } break;
   case Player::PLAYERSTATE_MOVEATTACK:
   {
   } break;
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

//Player가 당하는 입장이다.
void Player::Handle(const CollisionSystem::ActorTriggerEvent & event)
{
	if (event._entity1 != _entity)
	{
		return;
	}
	CollisionComponent & _collision = event._entity2.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
	case CollisionComponent::TRIGGER_TYPE_ENEMY :
	{
		if (_state != PLAYERSTATE_HURT && _state != PLAYERSTATE_DEAD)
		{
			_state = PLAYERSTATE_HURT;
			this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarTakingHit));
			_inCombat = true;
			_hp -= 50;
			if (_hp <= 0)
			{
				_state = PLAYERSTATE_DEAD;
				this->_pActionComp->_actionQueue.ClearQueue();
				this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarDying));
			}
		}

	} break;
	//오브젝트와 충돌했다
	case CollisionComponent::TRIGGER_TYPE_OBJECT:
	{
	} break;
	case CollisionComponent::TRIGGER_TYPE_DEFAULT:
	{
	} break;
	}
}

void Player::QueueAction(const Action & action)
{
   _pActionComp->_actionQueue.PushAction(action);
   _animationEnum = action._enum;
}

void Player::RepositionEntity(const TerrainTilePos & currentPos, const TerrainTilePos & prevPos)
{
	bool32 _chunkMoved = false;
	//청크가 다를때..... 터레인의 엔티티를 활성화, 비활성화한다
	if (currentPos._chunkX != prevPos._chunkX || currentPos._chunkZ != prevPos._chunkZ)
	{
		TERRAIN->ValidateTerrainChunks(currentPos, prevPos);
		_chunkMoved = true;
	}

	//타일이 다를....... tile의 entity벡터를 처리해주자
	if (currentPos._tileX != prevPos._tileX || currentPos._tileZ != prevPos._tileZ)
	{
		Terrain::TerrainChunk &refPrevChunk =  TERRAIN->GetChunkAt(prevPos._chunkX, prevPos._chunkZ);
		Terrain::TerrainTile &refPrevTile = refPrevChunk._tiles[Index2D(prevPos._tileX, prevPos._tileZ, TERRAIN_TILE_RES)];

		for (uint32 i = 0; i < refPrevTile._entities.size(); ++i)
		{
			if (refPrevTile._entities[i] == _entity)
			{
				refPrevTile._entities.erase(refPrevTile._entities.begin() + i);
				break;
			}
		}

		Terrain::TerrainChunk &refCurrentChunk =  TERRAIN->GetChunkAt(currentPos._chunkX, currentPos._chunkZ);
		Terrain::TerrainTile &refCurrentTile = refCurrentChunk._tiles[Index2D(currentPos._tileX, currentPos._tileZ, TERRAIN_TILE_RES)];
		refCurrentTile._entities.push_back(_entity);
	}
}

