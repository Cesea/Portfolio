#ifndef ANIMATION_UTILS_H
#define ANIMATION_UTILS_H

#define ANIMATION_BONE_MAX_NAME 64

namespace animation
{
	//Enums////////////////////////////////////////////////////////////////////////
	enum PriorityType 
	{
		ePriorityLow = 0,
		ePriorityHigh = 1,
		PRIORITY_FORCE_DWORD = 0x7fffffff
	};

	enum PlaybackType
	{
		ePlayLoop = 0,
		ePlayOnce = 1,
		ePlayPingPong = 2,
		PLAY_FORCE_DWORD = 0x7fffffff
	};

	enum EventType {
		eEventTrackSpeed = 0,
		eEventTrackWeight = 1,
		eEvetTrackPosition = 2,
		eEventTrackEnable = 3,
		ePriorityBlend = 4,
		EVENT_FORCE_DWORD = 0x7fffffff
	};

	enum CompressionFlags
	{
		eCompressDefault = 0,
		eCompressStrong = 1,
		COMPRESS_FORCE_DWORD = 0x7fffffff
	};

	enum CallbackSearchFlags 
	{
		eCallbackSearchExcludingInitialPosition = 1,
		eCallbackSearchBehindInitialPosition = 2,
		CALLBACK_SEARCH_FORCE_DWORD = 0x7fffffff
	};

	//Structs////////////////////////////////////////////////////////////////////////
	struct KeyCallback
	{
		float _time;
		void *_pCallbackData;
	};
	struct KeyVector3
	{
		float _time;
		Vector3 _value;
	};

	struct KeyQuaternion
	{
		float _time;
		Quaternion _value;
	};

	struct TrackDesc
	{
		PriorityType _priority;
		float _weight;
		float _speed;
		double _position;
		bool32 _enable;
	};

	/*struct Skeleton
	{
		uint16 *_hierachy{};
		Matrix *_localPoses{};
		Matrix *_globalPoses{};
		char *_names[ANIMATION_BONE_MAX_NAME];

		uint32 _numhierachy{};
	};*/
	//아마 사용 할 것 같다.... 
	//struct D3DXBONECOMBINATION 
	//{
	//	uint32 attribId;
	//	uint32 faceStart;
	//	uint32 faceCount;
	//	uint32 vertexStart;
	//	uint32 vertexCount;
	//	uint32 *boneId;
	//};

	//Functions////////////////////////////////////////////////////////////////////////

	//void BuildSkeleton(Skeleton *pSkeleton,);
	void UpdateSkeleton(video::Skeleton *pSkeleton, const Matrix *pWorldMatrix);
	//void RenderSkeleton();

}
#endif