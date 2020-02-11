
class ICamera
{
public:
    virtual void move( int x, int y ) = 0;
    virtual void moveX( int x ) = 0;
    virtual void moveY( int y ) = 0;
    virtual void noticeWhenMovingDone() = 0;
    virtual ~ICamera();
};
