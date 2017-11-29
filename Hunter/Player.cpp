#include "stdafx.h"
#include "Player.h"

#include "PlayerStates.h"

#define MAX_COMBO_COUNT 3

#include "stdafx.h"
#include "Player.h"

#include "PlayerStates.h"

#define MAX_COMBO_COUNT 3

Player::Player()
{
}

Player::~Player()
{
}

bool Player::CreateFromWorld(World & world)
{
   EventChannel channel;
   channel.Add<InputManager::KeyPressedEvent, Player>(*this);
   channel.Add<InputManager::KeyDownEvent, Player>(*this);
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

   _currentCommand._interpreted = true;

   //Plyer의 맴버 변수들을 셋팅해주자
   _combatToPeaceTimer.Reset(2.0f);
   _moveToStanceTimer.Reset(0.05f);
   _attackToStanceTimer.Reset(1.5f);

   _currentMovement._horizontal = HORIZONTAL_MOVEMENT_NONE;
   _currentMovement._vertical = VERTICAL_MOVEMENT_NONE;

   return true;
}

void Player::Update(float deltaTime)
{
   _pStateMachine->Update(deltaTime, _currentCommand);

   TransformComponent &transComp = _entity.GetComponent<TransformComponent>();
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
            if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
               MovementDown(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
            }
            else if(_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
            {
               MovementUp(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
            }
            else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
            {
               MovementLeft(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
            }
            else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
               MovementRight(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
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
               MovementLeft(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
            }
            else if(_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
               MovementRight(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
            }
            else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
            {
               MovementUp(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
            }
            else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
               MovementDown(_currentMovement);
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
            }
         }
         else if (_currentCommand._type == GAMECOMMAND_ACTION)
         {
            if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
            {
               _state = PLAYERSTATE_ATTACK;
               _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingLeft));
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

      Vector3 moveDelta{};
      Vector3 currentPos = transComp._position;

      Vector3 playerForward = transComp.GetForward();
      Vector3 playerRight = transComp.GetRight();


      //Vector3 forward = transComp.GetForward();
      if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
      {
         //moveDelta.z -= _speed;
         transComp.MovePositionSelf(playerForward * -_speed * deltaTime);
      }
      else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
      {
         //moveDelta.z += _speed;
         transComp.MovePositionSelf(playerForward * _speed * deltaTime);
      }
      else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
      {
         //moveDelta.x -= _speed;
         transComp.MovePositionSelf(playerRight * -_speed * deltaTime);
      }
      else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
      {
         //moveDelta.x += _speed;
         transComp.MovePositionSelf(playerRight * _speed * deltaTime);
      }
      /*currentPos += moveDelta * deltaTime;

      currentPos.y = TERRAIN->GetHeight(currentPos.x, currentPos.z);*/

      transComp.SetWorldPosition(transComp.GetWorldPosition().x, TERRAIN->GetHeight(transComp.GetWorldPosition().x, transComp.GetWorldPosition().z), transComp.GetWorldPosition().z);

      //transComp.SetWorldPosition(currentPos);

      //공격 모드가 아닐때
      if (false == _inCombat)
      {
         //만약 moveToStance 타이머가 울리면 stance상태로 돌아가라
         if (moveToStance)
         {
            _state = PLAYERSTATE_STANCE;
            _moveToStanceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStandingFree));
            return;
         }

         //일반 걷기
         //TODO : 옆으로 걷는거 모션 추가 해야 한다.
         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();
            if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
            {
               if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
                  MovementDown(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
               else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
                  MovementLeft(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
               }
               else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
                  MovementRight(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
               }
            }
            else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
               if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
                  MovementUp(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
               else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
                  MovementLeft(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
               }
               else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
                  MovementRight(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
               }
            }
            if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
                  MovementRight(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeLeft));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
                  MovementUp(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
                  MovementDown(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
            }
            else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
                  MovementLeft(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eStrafeRight));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
                  MovementUp(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
                  MovementDown(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalkingBack));
               }
            }
         }
      }
      //공격 모드 일때
      //공격 모드에서 앞으로 가는 모션을 어떤걸로 할까...
      else if (true == _inCombat)
      {
         if (moveToStance)
         {
            _state = PLAYERSTATE_STANCE;
            _moveToStanceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
            return;
         }

         if (_currentCommand._type == GAMECOMMAND_MOVE)
         {
            _moveToStanceTimer.Restart();
            if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
                  MovementRight(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
               }
               else if(_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
                  MovementDown(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
                  MovementUp(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
            }
            else if (_currentMovement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
                  MovementLeft(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
                  MovementDown(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
                  MovementUp(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
            }
            else if (_currentMovement._vertical == VERTICAL_MOVEMENT_DOWN)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
                  MovementLeft(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
               }
               else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
                  MovementRight(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_UP)
               {
                  MovementUp(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWalk));
               }
            }
            else if (_currentMovement._vertical == VERTICAL_MOVEMENT_UP)
            {
               if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_LEFT)
               {
                  MovementLeft(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingLeft));
               }
               else if (_currentCommand._movement._horizontal == HORIZONTAL_MOVEMENT_RIGHT)
               {
                  MovementRight(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarMovingRight));
               }
               else if (_currentCommand._movement._vertical == VERTICAL_MOVEMENT_DOWN)
               {
                  MovementDown(_currentMovement);
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarRetreat));
               }
            }
         }
      }
   } break;

   case Player::PLAYERSTATE_ATTACK:
   {
      if (_inCombat)
      {
         bool toStance = _attackToStanceTimer.Tick(deltaTime);
         if (toStance)
         {
            _state = PLAYERSTATE_STANCE;
            _attackToStanceTimer.Restart();
            _comboCount = 0;
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
         }
         if (_currentCommand._behavior._type == BEHAVIOR_ATTACK)
         {
            _comboCount++;
            if (_comboCount <= 3)
            {
               _attackToStanceTimer.Restart();
               if (_comboCount == 1)
               {
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingRight));
               }
               else if (_comboCount == 2)
               {
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarSwingHighStraigtDown));
               }
               else if (_comboCount == 3)
               {
                  _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarThrustMid));
               }
            }
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
            _attackToStanceTimer.Restart();
            _pStateMachine->QueueAction(PLAYER_ANIM(PlayerAnimationEnum::eWarCombatMode));
            return;
         }
      }
   } break;
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
   //ID3DXKeyframedAnimationSet *anim1;
   //ID3DXKeyframedAnimationSet *anim2;
   //ID3DXKeyframedAnimationSet *anim3;
   //ID3DXKeyframedAnimationSet *anim4;

   pController->GetAnimationSetByName(PlayerAnimationString[PlayerAnimationEnum::eWarSwingLeft], (ID3DXAnimationSet **)&anim0);

   //pController->GetAnimationSet(0, (ID3DXAnimationSet **)&anim0);
   //pController->GetAnimationSet(1, (ID3DXAnimationSet **)&anim1);
   //pController->GetAnimationSet(2, (ID3DXAnimationSet **)&anim2);
   //pController->GetAnimationSet(3, (ID3DXAnimationSet **)&anim3);
   //pController->GetAnimationSet(4, (ID3DXAnimationSet **)&anim4);

   _callbackData._animtionEnum = (PlayerAnimationEnum *)&_animationEnum;

   D3DXKEY_CALLBACK warSwingLeftKeys;
   warSwingLeftKeys.Time = anim0->GetPeriod() / 1.0f * anim0->GetSourceTicksPerSecond();
   warSwingLeftKeys.pCallbackData = (void *)&_callbackData;
   //anim0Keys[0].Time = 0;
   //anim0Keys[0].pCallbackData = (void *)&_callbackData;
   //anim0Keys[1].Time = anim0->GetPeriod() / 2.0f * anim0->GetSourceTicksPerSecond();
   //anim0Keys[1].pCallbackData = (void *)&_callbackData;

   AddCallbackKeysAndCompress(pController, anim0, 1, &warSwingLeftKeys, D3DXCOMPRESS_DEFAULT, 0.1f);
   //AddCallbackKeysAndCompress(pController, anim1, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   //AddCallbackKeysAndCompress(pController, anim2, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   //AddCallbackKeysAndCompress(pController, anim3, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
   //AddCallbackKeysAndCompress(pController, anim4, 2, anim0Keys, D3DXCOMPRESS_DEFAULT, 0.1f);
}

void Player::Handle(const InputManager::KeyPressedEvent & event)
{
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

void Player::Handle(const InputManager::KeyDownEvent & event)
{
   uint32 inputCode = event.code;

   if ('J' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
   }
   else if('L' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
   }
   else if('I' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_UP;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
   }
   else if ('K' == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_MOVE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_DOWN;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
   }
   else if (VK_SPACE == inputCode)
   {
      _currentCommand._type = GAMECOMMAND_JUMP;
   }
   /*else
   {
      _currentCommand._type = GAMECOMMAND_NONE;
      _currentCommand._movement._vertical = VERTICAL_MOVEMENT_NONE;
      _currentCommand._movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
   }*/
}

void Player::QueueAction(const Action & action)
{
   _pActionComp->_actionQueue.PushAction(action);
}

HRESULT PlayerCallbackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
    {
        PlayerCallbackData* pData = ( PlayerCallbackData* )pCallbackData;
      if (nullptr == pData)
      {
            return S_OK;
      }

      switch (*pData->_animtionEnum)
      {
      case PlayerAnimationEnum::eWarSwingLeft :
      {
         int a = 0;
      }break;

      }
        return S_OK;
    }


}