#ifndef LP_Base_H
#define LP_Base_H
#include <string>
#include "../../dxflib/src/dl_attributes.h"
#include "../../dxflib/src/dl_entities.h"
#include <vector>
#include <wx/dc.h>
#define LP_PI 3.1415926535897932384626433832795
// Windows XP can't handle the original MAX/MINDOUBLE's
#define RS_MAXDOUBLE 1.0E+10
#define RS_MINDOUBLE -1.0E+10
#define RS_TOLERANCE 1.0e-10
#define RS_TOLERANCE_ANGLE 1.0e-8
#define MAX_UNSIGNED_INT  4294967295
#define MAX_INT    2000000000//2147483640
#define MAX_NODE    2000  //每个图形的显示缓存线的最大节点数
using namespace std;
void __stdcall CommandHandle(const char *command,short error);
//对点进行操作的类。*************************************************************************
enum Graphics_TYPE
{
	LP_GRAPHBASE = 0,
	LP_POINT = 1,
	LP_LINE,
	LP_LINES,
	LP_POLYLINE,
	LP_SPLINE,
	LP_ELLIPSE,
	LP_TEXT,
	LP_MTEXT,
	LP_BLOCK,
	LP_INSERT,
	LP_LAYER,
	LP_SELECTED, //指向被选择的图形的一组指针
	LP_RECT,
};

class LP_Coordinate;
class LP_Graph
{
public:

    /**
     * Default constructor.
     */
    LP_Graph(void);

    /**
     * @param layer Layer name for this entity or NULL for no layer
     *              (every entity should be on a named layer!).
     * @param color Color number (0..256). 0 = BYBLOCK, 256 = BYLAYER.
     * @param width Line thickness. Defaults to zero. -1 = BYLAYER, 
     *               -2 = BYBLOCK, -3 = default width
     * @param lineType Line type name or "BYLAYER" or "BYBLOCK". Defaults
     *              to "BYLAYER"
     */
    LP_Graph(const DL_Attributes Attribut, const bool pick);

public:
    void setAttributes(const DL_Attributes pattribut);
    void setPick(const bool pick);
	bool getPick() const;
	virtual int getType();
	virtual void Draw(wxDC * dc, LP_Coordinate &cod) = 0;
    DL_Attributes getAttributes() const ;
    /**
     * Copies attributes (deep copies) from another attribute object.
     */
    LP_Graph& operator = (const LP_Graph& graph);

protected:
    DL_Attributes Attributes;
    bool IsPick;
	bool Effect;    //表示该曲线是否加工的标志。真为加工，假为不加工
	bool Pass;      //标志该曲线是否已加工。真为已加工，假为还未加工
	int linetype;   //绘制图形时实际使用的线型
	int type;
};
class LP_Point3 : public LP_Graph{
public:
    LP_Point3();
	LP_Point3(double px, double py);
    LP_Point3(double px, double py, double pz);
    void setPolar(double radius, double angle);
	void setD(double dx, double dy, double dz);
	void Draw(wxDC * dc, LP_Coordinate &cod);
	void minP(const LP_Point3 &cp);           //将此点的坐标值改变成两点中相应坐标值的较小值
	void maxP(const LP_Point3 &cp);          //将此点的坐标值改变成两点中相应坐标值的较大值
    double magnitude()const ; //求点到原点的距离

    double dotP(const LP_Point3& v1, const LP_Point3& v2) const ;

//	 * @return Cross product of two vectors.
    LP_Point3 crossP(const LP_Point3& v1, const LP_Point3& v2);
    double angle() const ; //求点与X轴正向的夹角
//	* @return The angle from this and the given coordinate (in rad).
    double angleTo(const LP_Point3& v) const ;

//	* @return The distance between this and the given coordinate.
    double distanceTo(const LP_Point3& v) const ;
//	* @return true is this vector is within the given range.
    bool isInRect(const LP_Point3& firstCorner, const LP_Point3& secondCorner);
	void DrawPick(wxDC *dc, LP_Coordinate cod);
//	* Moves this vector by the given offset. Equal to the operator +=.
    LP_Point3 move(LP_Point3& offset);
//	* Rotates this vector around 0/0 by the given angle.
    LP_Point3 rotate(double ang);
//	* Rotates this vector around the given center by the given angle.
    LP_Point3 rotate(LP_Point3 center, double ang);
//	* Scales this vector by the given factors with 0/0 as center.
	LP_Point3 scale(double xscale,double yscale,double zscale);
	LP_Point3 scale(LP_Point3 center,double xscale,double yscale,double zscale);
    LP_Point3 scale(LP_Point3 factor);
//	* Scales this vector by the given factors with the given center.
    LP_Point3 scale(LP_Point3 center, LP_Point3 factor);
//	* Mirrors this vector at the given axis.
    LP_Point3 mirror(LP_Point3 axisPoint1, LP_Point3 axisPoint2);
    LP_Point3 operator + (const LP_Point3& v) const;
    //* binary - operator.
    LP_Point3 operator - (const LP_Point3& v) const;
    LP_Point3 operator * (double s) const;
//	* binary / operator.
    LP_Point3 operator / (double s) const ;
//	* unary - operator.
    LP_Point3 operator - () const ;
//	* Scalarproduct (dot product).
    double dotP(const LP_Point3& v1, const LP_Point3& v2);
//	* += operator. Assert: both vectors must be valid.
    void operator += (const LP_Point3& v);
//	* -= operator
    void operator -= (const LP_Point3& v);
//	* *= operator
    void operator *= (double s) ;
//	* == operator
    bool operator == (const LP_Point3& v) const ;
//  * != operator
	bool operator != (const LP_Point3& v) const;
    //= operator
    LP_Point3& operator = (const LP_Point3& point3);
public:
    double x;
    double y;
    double z;
    bool valid;
};

class LP_Rect : public LP_Graph{
public:
	LP_Rect(void);
	LP_Rect(LP_Point3 leftbottom, LP_Point3 righttop);
	LP_Rect(double lbx, double lby, double rtx, double rty);   //用左下角点和右上角点构造一个矩形
	bool isInRect(LP_Rect rect);     // 图形是否在所给矩形内
	bool isContactsRect(const LP_Rect& rect);//图形是否与所给矩形相交
	bool LineClip(double &x1,double &y1,double &x2,double &y2/*,double XL,double XR,double YB,double YT*/);//计算直线在矩形内的线段
	LP_Rect operator + (const LP_Rect& r) const ;    //返回两个矩形相加的矩形，即用两个矩形的最大边线构造的新矩形
	//
//	* Scales this vector by the given factors with 0/0 as center.
    void scale(LP_Point3 factor);
//	* Scales this vector by the given factors with the given center.
    void scale(LP_Point3 center, LP_Point3 factor);
//	* Mirrors this vector at the given axis.
	void move(LP_Point3 & ref);
	bool checkPick(LP_Point3 wp,LP_Coordinate &cod);
	void Draw(wxDC * dc, LP_Coordinate &cod);
	void DrawRuler(wxDC *dc, LP_Coordinate &cod);
protected:
    int encode(double x,double y);//用显示矩形裁剪直线的编码函数
public:
	LP_Point3 LeftBottom;      //矩形的左下角点
	LP_Point3 RightTop;        //矩形的右上角点
};
class LP_Selected;
class LP_Coordinate{    ///坐标及坐标变换类
public:
//	LP_Coordinate(void);
    LP_Coordinate(double cx = 200, double cy = 200, double cwidth = 1024, double cheight = 768, double cscale = 8, double canglexz = 0, double cangleyz = LP_PI/2, double calpha = 1, double csmooth = 1, bool crecreate = false);   //x = 200; y = 200; width = 1024; height = 768; scale = 8; anglexz = 0; angleyz = LP_PI/2; alpha = 1;smooth = 1;recreate = false;
    LP_Coordinate(double lpx, double lpy, double lpscale, double lpalpha);
	LP_Coordinate(LP_Coordinate& cod);
    
    void setSmooth(double smo);
	void setScale(double sca);
	void setOriginInScreen(double sx,double sy); //设置坐标原点在屏幕坐标中的位置
	void setOriginInScreen(wxPoint pt);
	void setWidth(double wi);  //设置显示矩形在世界坐标系统中的的宽度
	void setHeight(double he); //设置显示矩形在世界坐标系统中的的高度

	double getSmooth();
	double getScale();
	LP_Point3 getOriginInScreen();
	double getWidth();
	double getHeight();
	LP_Rect getViewRect();
	void setViewRect(LP_Rect vrect);

	inline double inch2mm(const double &inch);
	wxPoint toScreen(LP_Point3 &lp);
	void toScreenD(LP_Point3 &lp);//从世界坐标转到屏幕坐标的双精度版本
	wxPoint toScreen(double dx,double dy, double dz);
    LP_Point3 toWorld(double sx, double sy);
	LP_Rect getPickRect(LP_Point3 wp);
public:   //公开访问的变量
	bool recreate;
	double anglexz;  //屏幕平面与XZ轴的夹角
    double angleyz;  //屏幕平面与YZ轴的夹角
	wxPen pickpen;        //绘制被拾取的图形的画笔
	int nodesize;    //表示图形被拾取的节点框大小
	wxColor nodecolor; //表示图形被拾取的节点框的颜色
	wxBrush nodebrush;
	wxBitmap viewbmp;
	wxPen windowselpen;   //用于窗选的矩形边杠画笔
	wxPen rectselpen;     //用于拉框选的矩形边框画笔
	wxBrush windowselbrush;  //用于窗选的矩形填充颜色，图形处于窗口内的被选择
	wxBrush rectselbrush;    //用于矩形选的矩形填充颜色，图形与矩形相交的被选择
	wxPen rulerpen;  //绘制标尺时使用的画笔
	wxBrush rulerbrush; //绘制标尺时使用的画刷
	bool revsel;    //选择反选的标志，true为反选，false为选择添加
	bool drawhot;   //是否绘制热点的标志
	int tempcolor;

private:   //私有的变量	
	LP_Rect pickrect; //拾取框矩形。在世界坐标系中
	int pickrectsize; //拾取框矩形的大小。在屏幕上。以像素为单位
	double width;   //窗口区的宽度
    double height;  //窗口区的高度
	LP_Rect viewrect;
    double smooth;  //smooth必须大于零 定义为细分时线段的长度 值越小越光滑
    double alpha;  //视图的透视系数,  当前未用!
    double scale;  //物理坐标的缩放系数。
    double x;    //坐标原点在屏幕坐标中的位置。
    double y;
};
#endif