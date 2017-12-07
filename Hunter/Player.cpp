#include "stdafx.h"
#include "Player.h"

#include "Camera.h"

#include "DamageBox.h"

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

bool Player::CreateFromWorld(World & world, const Vector3 &pos)
{
   EventChannel channel;
   channel.Add<InputManager::KeyDownEvent, Player>(*this);
   channel.Add<InputManager::KeyReleasedEvent, Player>(*this);
   channel.Add<InputManager::KeyPressedEvent, Player>(*this);
   channel.Add<InputManager::MousePressedEvent, Player>(*this);

   channel.Add<CollisionSystem::ActorTriggerEvent, Player>(*this);

   _entity = world.CreateEntity();

   TransformComponent &transComp = _entity.AddComponent<TransformComponent>();
   transComp._position = Vector3(pos.x, TERRAIN->GetHeight(pos.x, pos.z), pos.z);
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


   _pSwordFrame = (Bone *)D3DXFrameFind(pAnimation->_pSkinnedMesh->_pRootBone, "_1H_sword01");
   if (_pSwordFrame  &&
	   _pSwordFrame->pMeshContainer)
   {
	   _pDamageBox = new DamageBox;

	   _pDamageBox->CreateFromWorld(world, pos);
	   TransformComponent &damageTrans = _pDamageBox->GetEntity().GetComponent<TransformComponent>();
	   CollisionComponent &damageCollision = _pDamageBox->GetEntity().GetComponent<CollisionComponent>();

	   transComp.AddChild(&damageTrans);

	   damageCollision._triggerType = CollisionComponent::TRIGGER_TYPE::TRIGGER_TYPE_PLAYER_DMGBOX;
	   damageCollision._duration = 100000.0f;
	   damageCollision._valid = false;

	   float range = 0.8f;

	   damageCollision._boundingBox.Init(Vector3(-range, -range, -range), Vector3(range, range, range));
	   damageCollision._dmg = 100;
   }

   //Plyer의 맴버 변수들을 셋팅해주자
   _combatToPeaceTimer.Reset(3.0f);
   _moveToStanceTimer.Reset(0.13f);
   _attackToStanceTimer.Reset(0.9f);

   _superArmorTimer.Reset(1.5f);

   _keyConfig._up = 'W';
   _keyConfig._down = 'S';
   _keyConfig._left = 'A';
   _keyConfig._right = 'D';

   return true;
}

void Player::Update(float deltaTime)
{
   MoveAndRotate(deltaTime);

   if (_superArmor == true)
   {
	   if (_superArmorTimer.Tick(deltaTime))
	   {
		   _superArmor = false;
		   _superArmorTimer.Restart();
	   }
   }
   CollisionComponent &damageCol = _pDamageBox->GetEntity().GetComponent<CollisionComponent>();

   switch (_state)
   {
   case Player::PLAYERSTATE_STANCE:
   {
	   if (false == _inCombat)
	   {
		   bool combatToPeace = _combatToPeaceTimer.Tick(deltaTime);
		   if (combatToPeace)
		   {
			   _inCombat = false;
			   _combatToPeaceTimer.Restart();
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
		   }
	   }
   } break;
   case Player::PLAYERSTATE_MOVE:
   {
	   bool moveToStance = false;
	   //인풋이 없을때만 MoveToStanceTimer를 작동시킨다....
	   if (IsMovementNone(_currentCommand._movement))
	   {
		   if (_moveToStanceTimer.Tick(deltaTime))
		   {
			   _state = PLAYERSTATE_STANCE;
			   _moveToStanceTimer.Restart();
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
		   }
	   }
   } break;

   case Player::PLAYERSTATE_ATTACK:
   {
	   if (_inCombat)
	   {
		   if (_canCombo &&
			   _attackToStanceTimer.Tick(deltaTime))
		   {
			   _state = PLAYERSTATE_STANCE;
			   _pDamageBox->GetEntity().GetComponent<CollisionComponent>()._valid = false;
			   _attackToStanceTimer.Restart();
			   _comboCount = 0;
			   _canCombo = false;
			   //this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
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
			   _attackToStanceTimer.Restart();
			   //_pActionComp->_actionQueue.ClearQueue();
			   //this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
		   }
	   }
   } break;

   case Player::PLAYERSTATE_MOVEATTACK:
   {
	   if (_inCombat)
	   {
		   if (_canCombo &&
			   _attackToStanceTimer.Tick(deltaTime))
		   {
			   _pDamageBox->GetEntity().GetComponent<CollisionComponent>()._valid = false;

			   _state = PLAYERSTATE_STANCE;
			   _attackToStanceTimer.Restart();
			   _pActionComp->_actionQueue.ClearQueue();

			   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
			   {
				   _state = PLAYERSTATE_MOVE;
				   //this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
			   }
			   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
			   {
				   _state = PLAYERSTATE_MOVE;
				   //this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
			   }
			   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
			   {
				   _state = PLAYERSTATE_MOVE;
				   //this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
			   }
			   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
			   {
				   _state = PLAYERSTATE_MOVE;
				   //this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
			   }
			   else if (IsMovementNone(_currentCommand._movement))
			   {
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			   }
		   }
	   }
   }break;

   //case Player::PLAYERSTATE_HURT:
   //{
   //}break;

   case Player::PLAYERSTATE_DEAD:
   {

   }break;
   }
   _prevCommand = _currentCommand;
   _currentCommand.Reset();

   TransformComponent &refDamageTrans = _pDamageBox->GetEntity().GetComponent<TransformComponent>();
   //Console::Log("%d\n", (int32)_pDamageBox->GetEntity().GetComponent<CollisionComponent>()._valid);
   _worldSwordPos = refDamageTrans.GetWorldPosition();
   Matrix forwardTrans;
   MatrixTranslation(&forwardTrans, -60.0f, 0.0f, 0.0f);
   MatrixMultiply(&forwardTrans,  &forwardTrans,  &_pSwordFrame->CombinedTransformationMatrix);
   Vec3TransformCoord(&_worldSwordPos, &_worldSwordPos, &forwardTrans);

   refDamageTrans.SetWorldPosition(_worldSwordPos);

   _channel.Broadcast<PlayerImformationEvent>(
	   PlayerImformationEvent(_pTransformComp->GetWorldPosition(), _state, _pTransformComp->GetForward(), _worldSwordPos));
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
		_state == PLAYERSTATE_HURT)
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
		toMove -= right;
	}
	else if (_animationEnum == PlayerAnimationEnum::eStrafeRight ||
		_animationEnum == PlayerAnimationEnum::eWarMovingRight)
	{
		toMove += right;
	}
	else if (_animationEnum == PlayerAnimationEnum::eWalk ||
		_animationEnum == PlayerAnimationEnum::eWarCharging)
	{
		toMove += forward;
	}
	else if (_animationEnum == PlayerAnimationEnum::eWalkingBack ||
		_animationEnum == PlayerAnimationEnum::eWarRetreat)
	{
		toMove -= forward;
	}
	else if (_state == PLAYERSTATE_MOVEATTACK)
	{
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

   if(_keyConfig._left == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
   }
   else if (_keyConfig._right == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
   }

   if(_keyConfig._up == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
   }
   else if (_keyConfig._down == inputCode)
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

   if(_keyConfig._left == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
   }
   else if (_keyConfig._right == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
   }

   if(_keyConfig._up == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
   }
   else if (_keyConfig._down == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
   }

   switch (_state)
   {
   case Player::PLAYERSTATE_STANCE:
   {
	   if (_keyConfig._up == inputCode)
	   {
		   _startForward = _pTransformComp->GetForward();
		   _runStartForward = _startForward;
		   _state = PLAYERSTATE_MOVE;
		   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
	   }
	   else if (_keyConfig._down == inputCode)
	   {
		   _startForward = _pTransformComp->GetForward();
		   _runStartForward = _startForward;
		   _state = PLAYERSTATE_MOVE;
		   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat)) :
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
	   }
	   else if (_keyConfig._left == inputCode)
	   {
		   _startForward = _pTransformComp->GetForward();
		   _runStartForward = _startForward;
		   _state = PLAYERSTATE_MOVE;
		   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft)) :
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
	   }
	   else if (_keyConfig._right == inputCode)
	   {
		   _startForward = _pTransformComp->GetForward();
		   _runStartForward = _startForward;
		   _state = PLAYERSTATE_MOVE;
		   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight)) :
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
	   }
   } break;
   case Player::PLAYERSTATE_MOVE:
   {
	   if (_keyConfig._up == inputCode)
	   {
		   if (_animationEnum == PlayerAnimationEnum::eWalkingBack ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat)
		   {
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eStrafeLeft ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingLeft)
		   {
			   _targetRotation = PI_DIV_4;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eStrafeRight ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingRight)
		   {
			   _targetRotation = -PI_DIV_4;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
		   }
	   }
	   else if (_keyConfig._down == inputCode)
	   {
		   if (_animationEnum == PlayerAnimationEnum::eWalk) 
		   {
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eStrafeLeft ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingLeft)
		   {
			   _targetRotation = -PI_DIV_4;
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eStrafeRight ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingRight)
		   {
			   _targetRotation = PI_DIV_4;
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
		   }
	   }
	   else if (_keyConfig._left == inputCode)
	   {
		   if (_animationEnum == PlayerAnimationEnum::eStrafeRight || 
			  _animationEnum == PlayerAnimationEnum::eWarMovingRight)
		   {
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWalk)
		   {
			   _targetRotation = PI_DIV_4;
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWalkingBack ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat)
		   {
			   _targetRotation = -PI_DIV_4;
		   }
	   }
	   else if (_keyConfig._right == inputCode)
	   {
		   if (_animationEnum == PlayerAnimationEnum::eStrafeLeft ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingLeft)
		   {
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWalk)
		   {
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWalkingBack ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat)
		   {
			   _targetRotation = PI_DIV_4;
		   }
	   }
	   else if (VK_SHIFT == inputCode)
	   {
		   _state = PLAYERSTATE_RUN;

		   if (_animationEnum == PlayerAnimationEnum::eWalk)
		   {
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCharging));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eWarRetreat ||
			   _animationEnum == PlayerAnimationEnum::eWalkingBack)
		   {
			   _camRotated = true;
			   _targetRotation = D3DX_PI;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCharging));
		   }
		   else if (_animationEnum == PlayerAnimationEnum::eStrafeLeft ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingLeft)
		   {
			   _camRotated = true;
			   _targetRotation = PI_DIV_2;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCharging));

		   }
		   else if (_animationEnum == PlayerAnimationEnum::eStrafeRight ||
			   _animationEnum == PlayerAnimationEnum::eWarMovingRight)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_2;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCharging));

		   }

	   }
   } break;
   case Player::PLAYERSTATE_RUN:
   {
	   if (_keyConfig._up == inputCode)
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation -= PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation += PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation += D3DX_PI;
		   }
	   }
	   else if (_keyConfig._down == inputCode)
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation += PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation -= PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation -= D3DX_PI;
		   }
	   }
	   else if (_keyConfig._left == inputCode)
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
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation -= D3DX_PI;
		   }
	   }
	   else if (_keyConfig._right == inputCode)
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation = -PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation = +PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation += D3DX_PI;
		   }
	   }
   } break;
   case Player::PLAYERSTATE_ATTACK:
	   break;
   case Player::PLAYERSTATE_BLOCK:
	   break;
   case Player::PLAYERSTATE_MOVEATTACK:
   {
	   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
	   {
		   if (_keyConfig._left == inputCode)
		   {
			   _targetRotation += PI_DIV_4;
		   }
		   else if (_keyConfig._right == inputCode)
		   {
			   _targetRotation -= PI_DIV_4;
		   }
	   }
   } break;
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
	   if (_keyConfig._up == inputCode)
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _targetRotation = 0.0f;
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _targetRotation = 0.0f;
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
		   }
	   }
	   else if (_keyConfig._down == inputCode)
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _targetRotation = 0.0f;
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _targetRotation = 0.0f;
			   _inCombat ? this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight)) :
				   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
		   }
	   }
	   else if (_keyConfig._left == inputCode)
	   {
		   if (_animationEnum == PlayerAnimationEnum::eWalk ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat || 
			   _animationEnum == PlayerAnimationEnum::eWalkingBack)
		   {
			   _targetRotation = 0.0f;
		   }
	   }
	   else if (_keyConfig._right == inputCode)
	   {
		   if (_animationEnum == PlayerAnimationEnum::eWalk ||
			   _animationEnum == PlayerAnimationEnum::eWarRetreat ||
			   _animationEnum == PlayerAnimationEnum::eWalkingBack)
		   {
			   _targetRotation = 0.0f;
		   }
	   }
   } break;
   case Player::PLAYERSTATE_RUN:
   {
	   if (_keyConfig._up == inputCode)
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation += PI_DIV_4;

		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation -= PI_DIV_4;
		   }
	   }
	   else if (_keyConfig._down == inputCode)
	   {
		   if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
		   {
			   _camRotated = true;
			   _targetRotation -= PI_DIV_4;
		   }
		   else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
		   {
			   _camRotated = true;
			   _targetRotation += PI_DIV_4;
		   }
	   }
	   else if (_keyConfig._left == inputCode)
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation -= PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation += PI_DIV_4;
		   }
	   }
	   else if (_keyConfig._right == inputCode)
	   {
		   if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
		   {
			   _camRotated = true;
			   _targetRotation += PI_DIV_4;
		   }
		   else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
		   {
			   _camRotated = true;
			   _targetRotation -= PI_DIV_4;
		   }
	   }
	   else if (VK_SHIFT == inputCode)
	   {
		   _state = PLAYERSTATE_MOVE;
		   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
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
	   if (_keyConfig._left == inputCode)
	   {

	   }
	   else if (_keyConfig._right == inputCode)
	   {

	   }
	   else if (_keyConfig._up == inputCode)
	   {

	   }
	   else if (_keyConfig._down == inputCode)
	   {

	   }
	   else if (VK_SHIFT == inputCode)
	   {

	   }

	   //if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
	   //{
		  // if (_keyConfig._left == inputCode)
		  // {
			 //  _targetRotation = 0;
		  // }
		  // else if (_keyConfig._right == inputCode)
		  // {
			 //  _targetRotation = 0;
		  // }
	   //}
   } break;
   }
}

void Player::Handle(const InputManager::MousePressedEvent & event)
{
   uint32 inputCode = event.code;

   switch (_state)
   {
   case Player::PLAYERSTATE_STANCE:
   {
	   if (MOUSE_BUTTON_LEFT == inputCode)
	   {
		   if (false == _inCombat)
		   {
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			   _inCombat = true;
		   }
		   else
		   {
			   _state = PLAYERSTATE_ATTACK;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
		   }
	   }
	   else if (MOUSE_BUTTON_RIGHT == inputCode)
	   {
		   if (false == _inCombat)
		   {
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
			   _inCombat = true;
		   }
		   else
		   {
			   _state = PLAYERSTATE_BLOCK;
			   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarShieldBlock));
		   }
	   }
   } break;
   case Player::PLAYERSTATE_MOVE:
   {
	   if (MOUSE_BUTTON_LEFT == inputCode)
	   {
		   _state = PLAYERSTATE_MOVEATTACK;
		   _inCombat = true;
		   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkSwingLeft));
	   }
	   else if (MOUSE_BUTTON_RIGHT == inputCode)
	   {
		   _state = PLAYERSTATE_BLOCK;
		   _inCombat = true;
		   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarShieldBlock));
	   }
   } break;
   case Player::PLAYERSTATE_RUN:
   {
	   if (MOUSE_BUTTON_LEFT == inputCode)
	   {
		   _state = PLAYERSTATE_MOVEATTACK;
		   _inCombat = true;
		   this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarWalkThrust));
	   }

   } break;
   case Player::PLAYERSTATE_ATTACK:
   {
	   if (MOUSE_BUTTON_LEFT == inputCode)
	   {
		   if (_canCombo)
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
   } break;
   case Player::PLAYERSTATE_MOVEATTACK:
   {
	   if (MOUSE_BUTTON_LEFT == inputCode)
	   {
		   if (_canCombo)
		   {
			   if (IsMovementNone(_currentCommand._movement))
			   {
				   _state = PLAYERSTATE_ATTACK;
			   }
			   else
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
	   }

   } break;
   case Player::PLAYERSTATE_HURT:
   {
   } break;
   }

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
	if (event._entity2 != _entity)
	{
		return;
	}

	CollisionComponent & _collision = event._entity1.GetComponent<CollisionComponent>();
	switch (_collision._triggerType)
	{
	case CollisionComponent::TRIGGER_TYPE_ENEMY_DMGBOX :
	{
		if (_state != PLAYERSTATE_HURT && _state != PLAYERSTATE_DEAD && !_superArmor)
		{
			_collision._valid = false;
			_collision._duration = -1.0f;
			MovementStop(_currentMovement);
			_state = PLAYERSTATE_HURT;
			_hp -= 50;
			_inCombat = true;
			if (_hp > 0)
			{
				this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarTakingHit), true);
			}
			else
			{
				_state = PLAYERSTATE_DEAD;
				this->_pActionComp->_actionQueue.ClearQueue();
				this->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarDying), true);
			}
		}
	} break;
	//오브젝트와 충돌했다
	case CollisionComponent::TRIGGER_TYPE_OBJECT:
	{
	} break;
	}
}

void Player::QueueAction(Action & action, bool cancle)
{
	action._cancle = cancle;
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

		bool removed = false;
		for (uint32 i = 0; i < refPrevTile._entities.size(); ++i)
		{
			if (refPrevTile._entities[i] == _entity)
			{
				refPrevTile._entities.erase(refPrevTile._entities.begin() + i);
				removed = true;
				break;
			}
		}

		if (removed)
		{
			Terrain::TerrainChunk &refCurrentChunk = TERRAIN->GetChunkAt(currentPos._chunkX, currentPos._chunkZ);
			Terrain::TerrainTile &refCurrentTile = refCurrentChunk._tiles[Index2D(currentPos._tileX, currentPos._tileZ, TERRAIN_TILE_RES)];
			refCurrentTile._entities.push_back(_entity);
			
		}
	}
}


