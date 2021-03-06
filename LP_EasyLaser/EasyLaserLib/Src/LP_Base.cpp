#include "LP_Base.h"
void __stdcall CommandHandle(const char *command,short error)
{
	switch(error)
	{
	case -1:
		printf(*command+"\a\nCommunication Error !"); break;
	case 0:
		break;
	case 1:
		printf(*command+"\a\nCommand Error !"); break;
	case 2:
		printf(*command+"\a\nRadius or chord is 0 !"); break;
	case 3:
		printf(*command+"\a\nLength is 0 or overflow !"); break;
	case 4:
		printf(*command+"\a\nVelocity or acceleration is less then 0 !");
		break;
	case 5:
		printf(*command+"\a\nChord is greater than diameter !"); break;
	case 7:
		printf(*command+"\a\nParameter error !"); break;
	default:
		printf(*command+"\a\nError Code = %d ",error); break;
	}
}
#define LEFT 1       //定义矩形裁剪直线时矩形的四个边
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
LP_Point3::LP_Point3(){
	x = 0;
	y = 0;
	z = 0;
	valid = false;
	type = LP_POINT;
}
LP_Point3::LP_Point3(double px, double py){
	x = px;
	y = py;
	z = 0;
	valid = true;
	type = LP_POINT;
}
LP_Point3::LP_Point3(double px, double py, double pz) {
	x = px;
	y = py;
	z = pz;
	valid = true;
	type = LP_POINT;
}
void LP_Point3::setPolar(double radius, double angle) {
	x = radius * cos(angle);
	y = radius * sin(angle);
	z = 0.0;
	valid = true;
}
void LP_Point3::setD(double dx, double dy, double dz){
	x = dx; y = dy; z = dz; valid = true;
}
void LP_Point3::Draw(wxDC * dc, LP_Coordinate &cod)
{
	LP_Rect vrect = cod.getViewRect(); //屏幕显示区域的矩形
	if(isInRect(vrect.LeftBottom,vrect.RightTop))
	{
		int colorindex = Attributes.getColor(); 		//	if(colorindex == 7){colorindex = 0;} 		wxPen pen(wxColor(dxfColors[colorindex][0] * 255,dxfColors[colorindex][1] * 255,dxfColors[colorindex][2] * 255)); 		dc->SetPen(pen); 		if(getPick()) 		{ 			dc->SetPen(cod.pickpen); 			dc->SetBrush(cod.nodebrush); 			if(cod.drawhot) 			{ 			    DrawPick(dc,cod); 			} 		}         dc->DrawPoint(cod.toScreen(x,y,0.0));
	}
}
void LP_Point3::minP(const LP_Point3 &cp){           //将此点的坐标值改变成两点中相应坐标值的较小值
	x = x < cp.x? x:cp.x;
	y = y < cp.y? y:cp.y;
	z = z < cp.z? z:cp.z;
}
void LP_Point3::maxP(const LP_Point3 &cp){          //将此点的坐标值改变成两点中相应坐标值的较大值
	x = x > cp.x? x:cp.x;
	y = y > cp.y? y:cp.y;
	z = z > cp.z? z:cp.z;
}
double LP_Point3::magnitude()const { //求点到原点的距离
	double length;
	if (!valid) {
		length = 0.0;
	}
	else {
		length = sqrt(x*x + y*y + z*z);
	}
	return length;
}
double LP_Point3::dotP(const LP_Point3& v1, const LP_Point3& v2) const {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
//	 * @return Cross product of two vectors.
LP_Point3 LP_Point3::crossP(const LP_Point3& v1, const LP_Point3& v2) {
	return LP_Point3(v1.y*v2.z - v1.z*v2.y,
		v1.z*v2.x - v1.x*v2.z,
		v1.x*v2.y - v1.y*v2.x);
}
double LP_Point3::angle() const { //求点与X轴正向的夹角
	double ret = 0.0;
	double m = magnitude();

	if (m>1.0e-6) {
		double dp = dotP(*this, LP_Point3(1.0, 0.0, 0));
		if (dp/m>=1.0) {
			ret = 0.0;
		}
		else {
			ret = acos( dp / m);
		}
		if (y<0.0) {
			ret = 2*LP_PI - ret;
		}
	}
	return ret;
}
//	* @return The angle from this and the given coordinate (in rad).
double LP_Point3::angleTo(const LP_Point3& v) const {
	if (!valid || !v.valid) {
		return 0.0;
	}
	else {
		return (v-(*this)).angle();
	}
}
//	* @return The distance between this and the given coordinate.
double LP_Point3::distanceTo(const LP_Point3& v) const {
	if (!valid || !v.valid) {
		return RS_MAXDOUBLE;
	}
	else {
		return (*this-v).magnitude();
	}
}
//	* @return true is this vector is within the given range.
bool LP_Point3::isInRect(const LP_Point3& firstCorner, 
						 const LP_Point3& secondCorner) {

							 double minX = min(firstCorner.x, secondCorner.x);
							 double maxX = max(firstCorner.x, secondCorner.x);
							 double minY = min(firstCorner.y, secondCorner.y);
							 double maxY = max(firstCorner.y, secondCorner.y);

							 return (x>=minX && x<=maxX && y>=minY && y<=maxY);
}
void LP_Point3::DrawPick(wxDC *dc, LP_Coordinate cod){
	wxPoint dp = cod.toScreen(x,y,z);
	int ps = cod.nodesize/2;
	dc->DrawRectangle(dp.x-ps,dp.y-ps, cod.nodesize, cod.nodesize);
}
//	* Moves this vector by the given offset. Equal to the operator +=.
LP_Point3 LP_Point3::move(LP_Point3& offset) {
	*this+=offset;
	return *this;
}
//	* Rotates this vector around 0/0 by the given angle.
LP_Point3 LP_Point3::rotate(double ang) {
	double r = magnitude();
	double a = angle() + ang;
	x = cos(a) * r;
	y = sin(a) * r;
	return *this;
}
//	* Rotates this vector around the given center by the given angle.
LP_Point3 LP_Point3::rotate(LP_Point3 center, double ang) {
	*this = center + (*this-center).rotate(ang);
	return *this;
}
LP_Point3 LP_Point3::scale(double xscale,double yscale,double zscale){
	x *= xscale;
	y *= yscale;
	z *= zscale;
	return *this;
}
LP_Point3 LP_Point3::scale(LP_Point3 center,double xscale,double yscale,double zscale){
	*this = center + (*this-center).scale(xscale,yscale,zscale);
	return *this;
}
//	* Scales this vector by the given factors with 0/0 as center.
LP_Point3 LP_Point3::scale(LP_Point3 factor) {
	return scale(factor.x,factor.y,factor.z);
}
//	* Scales this vector by the given factors with the given center.
LP_Point3 LP_Point3::scale(LP_Point3 center, LP_Point3 factor) {
	*this = center + (*this-center).scale(factor);
	return *this;
}
//	* Mirrors this vector at the given axis.
LP_Point3 LP_Point3::mirror(LP_Point3 axisPoint1, LP_Point3 axisPoint2) {

	double phi1 = axisPoint1.angleTo(*this);
	double phi2 = axisPoint1.angleTo(axisPoint2) - phi1;
	double r1 = axisPoint1.distanceTo(*this);
	double r2 = axisPoint2.distanceTo(*this);

	if (r1<1.0e-6 || r2<1.0e-6) {
		// point touches one axis point
	}
	else {
		setPolar(r1, phi1 + 2*phi2);
		(*this) += axisPoint1;
	}

	return *this;
}
LP_Point3 LP_Point3::operator + (const LP_Point3& v) const {
	return LP_Point3(x + v.x, y + v.y, z + v.z);
}
//* binary - operator.
LP_Point3 LP_Point3::operator - (const LP_Point3& v) const {
	return LP_Point3(x - v.x, y - v.y, z - v.z);
}
//	* binary * operator.
LP_Point3 LP_Point3::operator * (double s) const {
	return LP_Point3(x * s, y * s, z * s);
}
//	* binary / operator.
LP_Point3 LP_Point3::operator / (double s) const {
	return LP_Point3 (x / s, y / s, z / s);
}
//	* unary - operator.
LP_Point3 LP_Point3::operator - () const {
	return LP_Point3(-x, -y, -z);
}
//	* Scalarproduct (dot product).
double LP_Point3::dotP(const LP_Point3& v1, const LP_Point3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
//	* += operator. Assert: both vectors must be valid.
void LP_Point3::operator += (const LP_Point3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
//	* -= operator
void LP_Point3::operator -= (const LP_Point3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
//	* *= operator
void LP_Point3::operator *= (double s) {
	x *= s;
	y *= s;
	z *= s;
}
//	* == operator
bool LP_Point3::operator == (const LP_Point3& v) const {
	return (x==v.x && y==v.y && z==v.z && valid==v.valid);
}
//  * != operator
bool LP_Point3::operator != (const LP_Point3& v) const {
	if(x != v.x)
		return true;
	if(y != v.y)
		return true;
	if(z != v.z)
		return true;
	return false;
}
//= operator
LP_Point3& LP_Point3::operator = (const LP_Point3& point3){
	x = point3.x;
	y = point3.y;
	z = point3.z;
	valid = point3.valid;
	return *this;
}
LP_Coordinate::LP_Coordinate(double cx, double cy, double cwidth, double cheight, double cscale, double canglexz, double cangleyz, double calpha, double csmooth, bool crecreate){
	x = cx; 
	y = cy; 
	width = cwidth;
	height = cheight; 
	scale = cscale; 
	anglexz = canglexz;
	angleyz = cangleyz; 
	alpha = calpha;smooth = csmooth;
	recreate = crecreate;
	viewrect.LeftBottom = toWorld(0,height);
	viewrect.RightTop = toWorld(width,0);
	nodesize = 8;
	nodecolor = wxColor(0,255,0);
	pickpen.SetStyle(wxDOT);
	pickpen.SetColour(wxT("RED"));
	nodebrush = wxBrush(wxColour(wxT("GREEN")), wxSOLID);
	windowselpen.SetStyle(wxSOLID);
	windowselpen.SetColour(wxColor(0,255,0));
	rectselpen.SetStyle(wxDOT);
	rectselpen.SetColour(wxColor(0,255,255));
	windowselbrush = wxBrush(wxColour(wxT("BLUE")),wxSOLID);
	rectselbrush = wxBrush(wxColour(wxT("GREEN")),wxSOLID);
	rulerpen.SetStyle(wxSOLID);
	rulerpen.SetColour(wxColor(0,255,255));
	rulerbrush = wxBrush(wxColour(wxT("BLUE")),wxSOLID);
	revsel = false;
	drawhot = false;
	pickrectsize = 4;
}
LP_Coordinate::LP_Coordinate(double lpx, double lpy, double lpscale, double lpalpha){
    LP_Coordinate();
	x = lpx;
	y = lpy;
	scale = lpscale;
	alpha = lpalpha;
}
LP_Coordinate::LP_Coordinate(LP_Coordinate& cod){
	x = cod.x;
	y = cod.y;
	width = cod.getWidth();
	height = cod.getHeight(); 
	scale = cod.getScale(); 
	anglexz = cod.anglexz;
	angleyz = cod.angleyz; 
	alpha = cod.alpha;
	smooth = cod.getSmooth();
	recreate = cod.recreate;
	viewrect = cod.viewrect;
	pickrect = cod.pickrect;
	pickpen = cod.pickpen;
	nodesize = cod.nodesize;
	nodecolor = cod.nodecolor;
	nodebrush = cod.nodebrush;
	windowselpen = cod.windowselpen;
	rectselpen = cod.rectselpen;
	windowselbrush = cod.windowselbrush;
	rectselbrush = cod.rectselbrush;
}
inline double LP_Coordinate::inch2mm(const double &inch){return inch * 2.7;}
void LP_Coordinate::setSmooth(double smo){
	smooth = smo;
}
void LP_Coordinate::setScale(double sca){
	scale = sca;
}
void LP_Coordinate::setOriginInScreen(double sx,double sy){
	x = sx;
	y = sy;
}
void LP_Coordinate::setOriginInScreen(wxPoint pt){
	x = pt.x;
	y = pt.y;
}
void LP_Coordinate::setWidth(double wi){
	width = wi;
	viewrect.RightTop = toWorld(viewrect.LeftBottom.x + width,viewrect.LeftBottom.y + height);
}
void LP_Coordinate::setHeight(double he){
	height = he;
	viewrect.RightTop = toWorld(viewrect.LeftBottom.x,viewrect.LeftBottom.y);
}
double LP_Coordinate::getSmooth(){
	return smooth;
}
double LP_Coordinate::getScale(){
	return scale;
}
double LP_Coordinate::getWidth(){
	return width;
}
double LP_Coordinate::getHeight(){
	return height;
}
void LP_Coordinate::setViewRect(LP_Rect vrect){
	viewrect = vrect;
	width = viewrect.RightTop.x - viewrect.LeftBottom.x;
	height = viewrect.RightTop.y - viewrect.LeftBottom.y;
}
LP_Rect LP_Coordinate::getViewRect(){
	return viewrect;
}
LP_Point3 LP_Coordinate::getOriginInScreen(){
	LP_Point3 pt;
	pt.x = x;
	pt.y = y;
	pt.z = 0;
	pt.valid = true;
	return pt;
}
wxPoint LP_Coordinate::toScreen(LP_Point3 &lp){
	double sdx = lp.x*scale + x;
	double sdy = y - lp.y*scale;
	if(sdx >= MAX_INT)  //做饱和运算
	{
		sdx = MAX_INT;
	}
	if(sdy >=MAX_INT)
	{
		sdy = MAX_INT;
	}
	return wxPoint((int)(sdx), (int)(sdy));
}
void LP_Coordinate::toScreenD(LP_Point3 &lp){
	lp.x = lp.x*scale + x;
	lp.y =  y - lp.y*scale;
}
wxPoint LP_Coordinate::toScreen(double dx,double dy, double dz){
	double sdx = dx*scale + x;
	double sdy = y - dy*scale;
	if(sdx >= MAX_INT)
	{
		sdx = MAX_INT;
	}
	if(sdy >=MAX_INT)
	{
		sdy = MAX_INT;
	}
	return wxPoint((int)(sdx), (int)(sdy));
}
LP_Point3 LP_Coordinate::toWorld(double sx, double sy){
	return LP_Point3((sx - x)/scale , (y - sy)/scale, 0);
}
LP_Rect LP_Coordinate::getPickRect(LP_Point3 wp)
{
	if(scale <= 0)
	{
		scale = 1;/////////////////////////////////////////////
	}
	double psz = pickrectsize / scale;
	LP_Rect prt;
	prt.LeftBottom.x = wp.x - psz;
	prt.LeftBottom.y = wp.y - psz;
	prt.LeftBottom.valid = true;
	prt.RightTop.x = wp.x + psz;
	prt.RightTop.y = wp.y + psz;
	prt.RightTop.valid = true;
	return prt;
}
/**
* Default constructor.
*/
LP_Graph::LP_Graph(void) {
	this->Attributes.setLayer("");
	this->Attributes.setColor(256);
	this->Attributes.setWidth(1);
	this->Attributes.setLineType("BYLAYER");
	this->setPick(false);
	linetype = wxSOLID;
	Effect = true;
	Pass = false;
	type = LP_GRAPHBASE;
}

/**
* @param layer Layer name for this entity or NULL for no layer
*              (every entity should be on a named layer!).
* @param color Color number (0..256). 0 = BYBLOCK, 256 = BYLAYER.
* @param width Line thickness. Defaults to zero. -1 = BYLAYER, 
*               -2 = BYBLOCK, -3 = default width
* @param lineType Line type name or "BYLAYER" or "BYBLOCK". Defaults
*              to "BYLAYER"
*/
LP_Graph::LP_Graph(const DL_Attributes Attribut, const bool pick) {
	setAttributes(Attribut);
	linetype = wxSOLID;
	setPick(pick);
	linetype = wxSOLID;
	Effect = true;
	Pass = false;
	type = LP_GRAPHBASE;
}
void LP_Graph::setAttributes(const DL_Attributes pattribut){
	this->Attributes = pattribut;
	linetype = wxSOLID;
}
void LP_Graph::setPick(const bool pick){
	this->IsPick = pick;
}
DL_Attributes LP_Graph::getAttributes() const {
	return Attributes;
}
bool LP_Graph::getPick() const {
	return IsPick;
}
int LP_Graph::getType(){
	return type;
}
/**
* Copies attributes (deep copies) from another attribute object.
*/
LP_Graph& LP_Graph::operator = (const LP_Graph& graph) {
	setAttributes(graph.Attributes);
	setPick(graph.IsPick);
	return *this;
}
//矩形类定义开始
LP_Rect::LP_Rect(void){
	LeftBottom.setD(0,0,0);
	RightTop.setD(0,0,0);
	type = LP_RECT;
}
LP_Rect::LP_Rect(LP_Point3 leftbottom, LP_Point3 righttop){
	LeftBottom = leftbottom;
	RightTop = righttop;
	LeftBottom.valid = true;
	RightTop.valid = true;
	type = LP_RECT;
}
LP_Rect::LP_Rect(double lbx, double lby, double rtx, double rty){   //用左下角点和右上角点构造一个矩形
	LeftBottom.x = lbx;
	LeftBottom.y = lby;
	RightTop.x = rtx;
	RightTop.y = rty;
	LeftBottom.valid = true;
	RightTop.valid = true;
	type = LP_RECT;
}
bool LP_Rect::isInRect(LP_Rect rect){
	if(LeftBottom.valid && RightTop.valid){
		if(LeftBottom.isInRect(rect.LeftBottom, rect.RightTop) && RightTop.isInRect(rect.LeftBottom, rect.RightTop))
		{
			return true;
		}
	}
	return false;
}
bool LP_Rect::isContactsRect(const LP_Rect& rect){
//	if(LeftBottom.valid && RightTop.valid){
		LP_Point3 LB(max(LeftBottom.x,rect.LeftBottom.x),max(LeftBottom.y,rect.LeftBottom.y));  //取两个矩形的最小坐标值的较大值
		LP_Point3 RT(min(RightTop.x,rect.RightTop.x),min(RightTop.y,rect.RightTop.y));  //取两个矩形的最大坐标值的较小值
		if((LB.x <= RT.x)&&(LB.y <= RT.y)){//LB为相交矩形的左下角点，RT为相交矩形的右上角点。若不满足此条件，则两矩形不相交
		    return true;
		}
//	}
	return false;
}
bool LP_Rect::LineClip(double &x1,double &y1,double &x2,double &y2/*,double XL,double XR,double YB,double YT*/)
//x1,y1,线段第一个端点的坐标x2,y2,线段第二个端点的坐标 XL,XR,YB,YT;窗口的左右上下坐标值
{
	double XL,XR,YB,YT;
	XL = LeftBottom.x;
	XR = RightTop.x;
	YB = LeftBottom.y;
	YT = RightTop.y;
	int code1,code2,code;
	double x,y;
	x = x1; y = y1; //无意义语句。只是为了初始化变量，避免C4701警告
	code1=encode(x1,y1);
	code2=encode(x2,y2);
	while(code1!=0 ||code2!=0)
	{
		if((code1&code2) !=0) 
			return false;
		code = code1;
		if(code1==0) code = code2;
		if((LEFT&code) !=0)//交左边界
		{     
			x=XL;
			y=y1+(y2-y1)*(XL-x1)/(x2-x1);
		}
		else if((RIGHT&code) !=0)//右边界相交
		{    
			x=XR;
			y=y1+(y2-y1)*(XR-x1)/(x2-x1);
		}
		else if((BOTTOM&code) !=0)//与下边界相交
		{   
			y=YB;
			x=x1+(x2-x1)*(YB-y1)/(y2-y1);
		}
		else if((TOP & code) !=0)//与上边界相交
		{    
			y=YT;
			x=x1+(x2-x1)*(YT-y1)/(y2-y1);
		}
		if(code ==code1)
		{  
			x1=x;
			y1=y; 
			code1 =encode(x,y);
		}
		else
		{
			x2=x;
			y2=y; 
			code2 =encode(x,y);
		}
	}
	return true;
//	displayline（x1,y1,x2,y2);
}
int LP_Rect::encode(double x,double y){//用此矩形裁剪直线的编码函数
	int c = 0;
	if(x<LeftBottom.x)
		c = c|LEFT;
	else if(x>RightTop.x)
		c = c|RIGHT;
	if(y<LeftBottom.y)
		c = c|BOTTOM;
	else if(y>RightTop.y)
		c = c|TOP;
	return c;
}
LP_Rect LP_Rect::operator + (const LP_Rect& r) const {    //返加两个矩形相加的矩形，即用两个矩形的最大边线构造的新矩形
	if((r.LeftBottom.x == r.RightTop.x)&&(r.LeftBottom.y == r.RightTop.y)){
		return *this;
	}
	else if((LeftBottom.x == RightTop.x)&&(LeftBottom.y == RightTop.y)){
		return r;
	}
	return LP_Rect(min(min(r.LeftBottom.x,r.RightTop.x),min(LeftBottom.x,RightTop.x)),
		           min(min(r.LeftBottom.y,r.RightTop.y),min(LeftBottom.y,RightTop.y)),
		           max(max(r.LeftBottom.x,r.RightTop.x),max(LeftBottom.x,RightTop.x)),
		           max(max(r.LeftBottom.y,r.RightTop.y),max(LeftBottom.y,RightTop.y)));    //用最小x值和最小y值构造左下角点，用最大x值和最大y值构造右上角点。
}
//
void LP_Rect::move(LP_Point3 & ref){
	LeftBottom.move(ref);
	RightTop.move(ref);
}
//	* Scales this vector by the given factors with 0/0 as center.
void LP_Rect::scale(LP_Point3 factor) {
	LeftBottom.scale(factor);
	RightTop.scale(factor);
}
//	* Scales this vector by the given factors with the given center.
void LP_Rect::scale(LP_Point3 center, LP_Point3 factor) {
	LeftBottom.scale(center,factor);
	RightTop.scale(center,factor);
}
bool LP_Rect::checkPick(LP_Point3 wp,LP_Coordinate &cod){  //判断选择框是否在矩形的边线上
	LP_Rect prect = cod.getPickRect(wp);
	double picx = (prect.RightTop.x - prect.LeftBottom.x)*0.5;
	double picy = (prect.RightTop.y - prect.LeftBottom.y)*0.5;
	if(fabs(wp.x - LeftBottom.x) <= picx || fabs(wp.x - RightTop.x) <= picx){
		if(wp.y >= LeftBottom.y-picy && wp.y <= RightTop.y+picy){
			return true;
		}
	}
	if(fabs(wp.y - LeftBottom.y) <= picy || fabs(wp.y - RightTop.y) <= picy){
		if(wp.x >= LeftBottom.x-picx && wp.x <= RightTop.x+picx){
			return true;
		}
	}
	return false;
}
void LP_Rect::Draw(wxDC * dc, LP_Coordinate &cod){
	int colorindex = Attributes.getColor();
//	if(colorindex == 7){colorindex = 0;} //黑白转换，背景为非黑白时不需要
	LP_Point3 LeftTop(LeftBottom.x,RightTop.y,0);
	LP_Point3 RightBottom(RightTop.x,LeftBottom.y,0);
	wxPen pen(wxColor(dxfColors[colorindex][0] * 255,dxfColors[colorindex][1] * 255,dxfColors[colorindex][2] * 255));
	pen.SetStyle(wxSHORT_DASH);
	dc->SetPen(pen);
	dc->DrawLine(cod.toScreen(LeftBottom), cod.toScreen(LeftTop));
	dc->DrawLine(cod.toScreen(LeftTop), cod.toScreen(RightTop));
	dc->DrawLine(cod.toScreen(RightTop), cod.toScreen(RightBottom));
	dc->DrawLine(cod.toScreen(RightBottom), cod.toScreen(LeftBottom));
}
void LP_Rect::DrawRuler(wxDC *dc, LP_Coordinate &cod)
{
	double scale = cod.getScale();
	double tempscale = scale; 
	double mnum;         //Larger or narrow 放大或缩小的系数。相当于scale的冥次
	mnum = log10(tempscale);
    tempscale = pow(10.0,mnum - (int)mnum);
	if(tempscale < 0.2)          //圆整缩放系数，圆整后的数越大，标尺的刻线越密
	{
		tempscale = 0.05;
	}
	else if(tempscale < 0.5)
	{
		tempscale = 0.1;
	}
	else if(tempscale < 1)
	{
		tempscale = 0.2;
	}
	else if(tempscale < 2)
	{
		tempscale = 0.5;
	}
	else if(tempscale < 5)
	{
		tempscale = 1;
	}
	else
	{
		tempscale = 2;
	}
	LP_Rect rrect = cod.getViewRect();
	double delta = 1/(tempscale * pow(10.0,(int)mnum));  //标尺的一个小格代表的长度，确保tempscale是一个大于0小于10的数，否则可能出错
	double ystart = LeftBottom.y;
	double ysignstart = ystart;
	double yend = RightTop.y;
	double xstart = LeftBottom.x;
	double xsignstart = xstart;
	double xend = RightTop.x;
	int xcount = 0;
	int ycount = 0;
	double temps = 0;
	if(ystart >= rrect.RightTop.y)      //计算刻度值在世界坐标中的起点**************************
	{
		ystart = rrect.RightTop.y-12.0/scale;
	}
	else if(ystart <= rrect.LeftBottom.y)
	{
		ystart = rrect.LeftBottom.y+12.0/scale;
	}
	temps = ystart - LeftBottom.y;
	ysignstart = ystart - fmod(temps,delta*10.0); //取刻度值的整数倍为起点

	if(xstart >= rrect.RightTop.x)
	{
		xstart = rrect.RightTop.x-12.0/scale;
	}
	else if(xstart <= rrect.LeftBottom.x)
	{
		xstart = rrect.LeftBottom.x+12.0/scale;
	}
	temps = xstart - LeftBottom.x;
	xsignstart = xstart - fmod(temps,delta*10.0);//取刻度值的整数倍为起点
//**********************************************************************************************
	if(xend > rrect.RightTop.x)                                   //计算刻度值在世界坐标中的终点
	{
		xend = rrect.RightTop.x;
	}
	if(yend > rrect.RightTop.y)
	{
		yend = rrect.RightTop.y;
	}  //***************************************************************************************
	dc->SetPen(cod.rulerpen); 	dc->SetBrush(cod.rulerbrush);
	wxFont font(10, wxFONTFAMILY_SWISS, wxNORMAL, wxBOLD);  //设置文字字体
    dc->SetFont(font);
    dc->SetBackgroundMode(wxTRANSPARENT);
    dc->SetTextForeground(*wxBLACK);
    dc->SetTextBackground(*wxWHITE);
    //dc->DrawText(text, pt);
	wxString text = wxT("0");
	wxCoord w, h;
	wxPoint numpt;
	double rnum,textstartptx,textendpty;
	dc->GetTextExtent(text, &w, &h);
	if(LeftBottom.isInRect(rrect.LeftBottom,rrect.RightTop))
	{
		dc->DrawText(wxT("0"), cod.toScreen(LeftBottom.x - (h/2/scale),LeftBottom.y,0.0));
	}

	dc->DrawLine(cod.toScreen(xstart,ystart,0.0),cod.toScreen(xstart,yend,0.0));  //绘制Y方向坐标轴
	wxString fstr,afstr;
	int iend;
	fstr = wxT("%.");                //组装格式化字符串,以使能显示小数点后的所有非零数字
	iend = (delta < 1)?  -ceil(log10(delta)):0;
	if(iend < 0)    //格式化字符串不能为负数
	{
		iend = 0;
	}
	fstr += +wxString::Format("%d",iend);
	fstr += wxT("f");
	for(double c = ysignstart; c <= yend; c+=delta)
	{
		if(ycount >= 10)
		{
		    dc->DrawLine(cod.toScreen(xstart-10/scale,c,0.0),cod.toScreen(xstart,c,0.0));
			ycount = 0;
			double rnum = c - LeftBottom.y;
			text= wxString::Format(fstr,rnum);
            dc->GetTextExtent(text, &w, &h);
            textstartptx = xstart-(10+w)/scale;
			if(textstartptx < rrect.LeftBottom.x)
			{
				textstartptx = xstart;
			}
			else
			{
				textstartptx = xstart-(10+w)/scale;
			    
			}
			numpt = cod.toScreen(textstartptx,c + (h/2/scale),0.0);
			dc->DrawText(text, numpt);
		}
		else
		{
			if(ycount == 5)
			{
				dc->DrawLine(cod.toScreen(xstart-7/scale,c,0.0),cod.toScreen(xstart,c,0.0));
			}
			else
			{
			    dc->DrawLine(cod.toScreen(xstart-5/scale,c,0.0),cod.toScreen(xstart,c,0.0));
			}
		}
		ycount ++;
	}
	rnum = RightTop.y - LeftBottom.y;  //输出图形的Y总高
	afstr = wxT("%.");                //组装格式化字符串,以使能显示小数点后的所有非零数字
	iend = ceil(fabs(log10(rnum)));
	iend = iend > 4 ? 4:iend;
	if(iend < 0)
	{
		iend = 0;
	}
	afstr += +wxString::Format("%d",iend);
	afstr += wxT("f");
	text= wxString::Format(afstr,rnum);
    dc->GetTextExtent(text, &w, &h);
	textstartptx = xstart-(10+w)/scale;      //判断字符能不能在图形区域显示出来
	if(textstartptx < rrect.LeftBottom.x)
	{
		textstartptx = xstart;              //不能则调整
	}
	else
	{
		textstartptx = xstart-(10+w)/scale;		    
	}
	numpt = cod.toScreen(textstartptx,RightTop.y + (h/2/scale),0.0);
	dc->DrawText(text, numpt);    //输出图形的总高结束
	dc->DrawLine(cod.toScreen(xstart,ystart,0.0),cod.toScreen(xend,ystart,0.0));   //绘制X方向坐标轴
	for(double c = xsignstart; c <= xend; c+=delta)
	{
		if(xcount >= 10)
		{
	        dc->DrawLine(cod.toScreen(c,ystart-10/scale,0.0),cod.toScreen(c,ystart,0.0));
			xcount = 0;
			rnum = c - LeftBottom.x;
			text= wxString::Format(fstr,rnum);
			dc->GetTextExtent(text, &w, &h);
            textendpty = ystart - (h*1.4)/scale;       //控制X轴标尺字符的纵向偏移量
			if(textendpty < rrect.LeftBottom.y)
			{
				textendpty = ystart+(h*0.7/scale);
				numpt = cod.toScreen(c - (h/2/scale),textendpty,0.0);
			    dc->DrawRotatedText(text, numpt, 45.00);
			}
			else
			{
				textendpty = ystart-h*0.5/scale;
				numpt = cod.toScreen(c + (h/2/scale),textendpty,0.0);
			    dc->DrawRotatedText(text, numpt, -45.00);
			    
			}
		}
		else
		{
			if(xcount == 5)
			{
				dc->DrawLine(cod.toScreen(c,ystart-7/scale,0.0),cod.toScreen(c,ystart,0.0));
			}
			else
			{
			    dc->DrawLine(cod.toScreen(c,ystart-5/scale,0.0),cod.toScreen(c,ystart,0.0));
			}
		}
		xcount ++;
	}
	rnum = RightTop.x - LeftBottom.x;   //输出图形的X总宽
	afstr = wxT("%.");                //组装格式化字符串,以使能显示小数点后的所有非零数字
	iend = ceil(fabs(log10(rnum)));
	iend = iend > 4 ? 4:iend;
	if(iend < 0)
	{
		iend = 0;
	}
	afstr += +wxString::Format("%d",iend);
	afstr += wxT("f");
	text= wxString::Format(afstr,rnum);
	dc->GetTextExtent(text, &w, &h);
	textendpty = ystart - (h*1.4)/scale;
	if(textendpty < rrect.LeftBottom.y)
	{
		textendpty = ystart+(h*0.7/scale);
		numpt = cod.toScreen(RightTop.x - (h/2/scale),textendpty,0.0);
		dc->DrawRotatedText(text, numpt, 45.00);
	}
	else
	{
		textendpty = ystart-h*0.5/scale;
		numpt = cod.toScreen(RightTop.x + (h/2/scale),textendpty,0.0);
		dc->DrawRotatedText(text, numpt, -45.00);

	}
	//numpt = cod.toScreen(RightTop.x + (h/2/scale),ystart-15/scale,0.0);
	//dc->DrawRotatedText(text, numpt, -45.00);
}