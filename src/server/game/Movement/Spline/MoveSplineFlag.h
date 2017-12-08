#ifndef TRINITYSERVER_MOVESPLINEFLAG_H
#define TRINITYSERVER_MOVESPLINEFLAG_H

#include <string>

namespace Movement
{
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

    class MoveSplineFlag
    {
    public:
#ifdef LICH_KING
        enum eFlags
        {
            None = 0x00000000,
            // x00-xFF(first byte) used as animation Ids storage in pair with Animation flag
            Done = 0x00000100,
            Falling = 0x00000200,           // Affects elevation computation, can't be combined with Parabolic flag
            No_Spline = 0x00000400,
            Parabolic = 0x00000800,           // Affects elevation computation, can't be combined with Falling flag
            CanSwim = 0x00001000,
            Flying = 0x00002000,           // Smooth movement(Catmullrom interpolation mode), flying animation
            OrientationFixed = 0x00004000,           // Model orientation fixed
            Final_Point = 0x00008000,
            Final_Target = 0x00010000,
            Final_Angle = 0x00020000,
            Catmullrom = 0x00040000,           // Used Catmullrom interpolation mode
            Cyclic = 0x00080000,           // Movement by cycled spline
            Enter_Cycle = 0x00100000,           // Everytimes appears with cyclic flag in monster move packet, erases first spline vertex after first cycle done
            Animation = 0x00200000,           // Plays animation after some time passed
            Frozen = 0x00400000,           // Will never arrive
            TransportEnter = 0x00800000,
            TransportExit = 0x01000000,
            Unknown7 = 0x02000000,
            Unknown8 = 0x04000000,
            Backward = 0x08000000,
            Unknown10 = 0x10000000,
            Unknown11 = 0x20000000,
            Unknown12 = 0x40000000,
            Unknown13 = 0x80000000,

            // Masks
            Mask_Final_Facing = Final_Point | Final_Target | Final_Angle,
            // animation ids stored here, see AnimType enum, used with Animation flag
            Mask_Animations = 0xFF,
            // flags that shouldn't be appended into SMSG_MONSTER_MOVE\SMSG_MONSTER_MOVE_TRANSPORT packet, should be more probably
            Mask_No_Monster_Move = Mask_Final_Facing | Mask_Animations | Done,
            // CatmullRom interpolation mode used
            Mask_CatmullRom = Flying | Catmullrom,
            // Unused, not suported flags
            Mask_Unused = No_Spline | Enter_Cycle | Frozen | Unknown7 | Unknown8 | Unknown10 | Unknown11 | Unknown12 | Unknown13
        };
#else
        enum eFlags
        {
            None                = 0x00000000,
            Done                = 0x00000001,
            Falling             = 0x00000002,           // Affects elevation computation
            Unknown3            = 0x00000004,
            Unknown4            = 0x00000008,
            Unknown5            = 0x00000010,
            Unknown6            = 0x00000020,
            Unknown7            = 0x00000040,
            Unknown8            = 0x00000080,
            Runmode             = 0x00000100,
            Flying              = 0x00000200,           // Smooth movement(Catmullrom interpolation mode), flying animation
            No_Spline           = 0x00000400,
            Unknown12           = 0x00000800,
            Unknown13           = 0x00001000,
            Unknown14           = 0x00002000,
            Unknown15           = 0x00004000,
            Unknown16           = 0x00008000,
            Final_Point         = 0x00010000,
            Final_Target        = 0x00020000,
            Final_Angle         = 0x00040000,
            Unknown19           = 0x00080000,           // exists, but unknown what it does
            Cyclic              = 0x00100000,           // Movement by cycled spline
            Enter_Cycle         = 0x00200000,           // Everytimes appears with cyclic flag in monster move packet, erases first spline vertex after first cycle done
            Frozen              = 0x00400000,           // Will never arrive
            Unknown23           = 0x00800000,
            Unknown24           = 0x01000000,
            Unknown25           = 0x02000000,           // exists, but unknown what it does
            Unknown26           = 0x04000000,
            Unknown27           = 0x08000000,
            Unknown28           = 0x10000000,
            Unknown29           = 0x20000000,
            Unknown30           = 0x40000000,
            Unknown31           = 0x80000000,

            // Masks
            Mask_Final_Facing   = Final_Point | Final_Target | Final_Angle,
            // flags that shouldn't be appended into SMSG_MONSTER_MOVE\SMSG_MONSTER_MOVE_TRANSPORT packet, should be more probably
            Mask_No_Monster_Move = Mask_Final_Facing | Done,
            // CatmullRom interpolation mode used
            Mask_CatmullRom     = Flying /*| Catmullrom*/
        };
#endif
        inline uint32& raw() { return (uint32&)*this; }
        inline const uint32& raw() const { return (const uint32&)*this; }

        MoveSplineFlag() { raw() = 0; }
        MoveSplineFlag(uint32 f) { raw() = f; }
        MoveSplineFlag(const MoveSplineFlag& f) { raw() = f.raw(); }

        // Constant interface

        bool isSmooth() const { return raw() & Mask_CatmullRom; }
        bool isLinear() const { return !isSmooth(); }
        bool isFacing() const { return raw() & Mask_Final_Facing; }

//        uint8 getAnimationId() const { return animId; }
        bool hasAllFlags(uint32 f) const { return (raw() & f) == f; }
        bool hasFlag(uint32 f) const { return (raw() & f) != 0; }
        uint32 operator & (uint32 f) const { return (raw() & f); }
        uint32 operator | (uint32 f) const { return (raw() | f); }
        std::string ToString() const;

        // Not constant interface

        void operator &= (uint32 f) { raw() &= f; }
        void operator |= (uint32 f) { raw() |= f; }

#ifdef LICH_KING
        void EnableAnimation(uint8 anim) { raw() = (raw() & ~(Mask_Animations | Falling | Parabolic)) | Animation | anim; }
        void EnableParabolic() { raw() = (raw() & ~(Mask_Animations | Falling | Animation)) | Parabolic; }
        void EnableFalling() { raw() = (raw() & ~(Mask_Animations | Parabolic | Flying | Animation)) | Falling; }
        void EnableFlying() { raw() = (raw() & ~(Falling | Catmullrom)) | Flying; }
        void EnableCatmullRom() { raw() = (raw() & ~Flying) | Catmullrom; }
        void EnableTransportEnter() { raw() = (raw() & ~TransportExit) | TransportEnter; }
        void EnableTransportExit() { raw() = (raw() & ~TransportEnter) | TransportExit; }
#else
        void EnableFalling() { raw() = (raw() & ~(Flying)) | Falling; }
        void EnableFlying() { raw() = (raw() & ~(Falling)) | Flying; }
#endif
        void EnableFacingPoint() { raw() = (raw() & ~Mask_Final_Facing) | Final_Point; }
        void EnableFacingAngle() { raw() = (raw() & ~Mask_Final_Facing) | Final_Angle; }
        void EnableFacingTarget() { raw() = (raw() & ~Mask_Final_Facing) | Final_Target; }

#ifdef LICH_KING
        uint8 animId : 8;
        bool done : 1;
        bool falling : 1;
        bool no_spline : 1;
        bool parabolic : 1;
        bool walkmode : 1;
        bool flying : 1;
        bool orientationFixed : 1;
        bool final_point : 1;
        bool final_target : 1;
        bool final_angle : 1;
        bool catmullrom : 1;
        bool cyclic : 1;
        bool enter_cycle : 1;
        bool animation : 1;
        bool frozen : 1;
        bool transportEnter : 1;
        bool transportExit : 1;
        bool unknown7 : 1;
        bool unknown8 : 1;
        bool backward : 1;
        bool unknown10 : 1;
        bool unknown11 : 1;
        bool unknown12 : 1;
        bool unknown13 : 1;
#else
        bool done          : 1;
        bool falling       : 1;
        bool unknown3      : 1;
        bool unknown4      : 1;
        bool unknown5      : 1;
        bool unknown6      : 1;
        bool unknown7      : 1;
        bool unknown8      : 1;
        bool walkmode      : 1;
        bool flying        : 1;
        bool no_spline     : 1;
        bool unknown12     : 1;
        bool unknown13     : 1;
        bool unknown14     : 1;
        bool unknown15     : 1;
        bool unknown16     : 1;
        bool final_point   : 1;
        bool final_target  : 1;
        bool final_angle   : 1;
        bool unknown19     : 1;
        bool cyclic        : 1;
        bool enter_cycle   : 1;
        bool frozen        : 1;
        bool unknown23     : 1;
        bool unknown24     : 1;
        bool unknown25     : 1;
        bool unknown26     : 1;
        bool unknown27     : 1;
        bool unknown28     : 1;
        bool unknown29     : 1;
        bool unknown30     : 1;
        bool unknown31     : 1;
#endif
    };
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif
}

#endif // TRINITYSERVER_MOVESPLINEFLAG_H
