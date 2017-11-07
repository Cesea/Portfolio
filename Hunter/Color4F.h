#ifndef COLOR4F_H
#define COLOR4F_H

class Color4F : public D3DXCOLOR
{
public :
	Color4F() {}
	Color4F(DWORD argb);
	Color4F(CONST FLOAT *);
	Color4F(CONST D3DXFLOAT16 *);
	Color4F(CONST D3DCOLORVALUE&);
	Color4F(FLOAT r, FLOAT g, FLOAT b, FLOAT a);

	// casting
	operator DWORD () const;

	operator FLOAT* ();
	operator CONST FLOAT* () const;

	operator D3DCOLORVALUE* ();
	operator CONST D3DCOLORVALUE* () const;

	operator D3DCOLORVALUE& ();
	operator CONST D3DCOLORVALUE& () const;

	// assignment operators
	Color4F& operator += (CONST Color4F&);
	Color4F& operator -= (CONST Color4F&);
	Color4F& operator *= (FLOAT);
	Color4F& operator /= (FLOAT);

	// unary operators
	Color4F operator + () const;
	Color4F operator - () const;

	// binary operators
	Color4F operator + (CONST Color4F&) const;
	Color4F operator - (CONST Color4F&) const;
	Color4F operator * (FLOAT) const;
	Color4F operator / (FLOAT) const;

	friend Color4F operator * (FLOAT, CONST Color4F&);

	BOOL operator == (CONST Color4F&) const;
	BOOL operator != (CONST Color4F&) const;

};

#endif