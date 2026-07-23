#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"

float parameter = 0.0f;
TransformedView tv;

Vec2 Function_2D(float lambda){
	return (Vec2){
		0.0f + cosf(parameter) + cosf(parameter) * lambda * lambda,
		0.0f + sinf(parameter) + sinf(parameter) * lambda
	};
	//return Vec2_Add(
	//	(Vec2){ 0.0f,0.0f },
	//	Vec2_Mulf(
	//		(Vec2){ parameter,1.0f },
	//		lambda
	//	)
	//);
}

void Setup(AlxWindow* w){
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
}

void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

	if(Stroke(ALX_KEY_UP).DOWN){
		parameter += 1.0f * w->ElapsedTime;
	}else if(Stroke(ALX_KEY_DOWN).DOWN){
		parameter -= 1.0f * w->ElapsedTime;
	}

	Clear(BLACK);

	Vec2 wvls = { 0.0f,TransformedView_ScreenWorldY(&tv,0.0f) };
	Vec2 wvle = { 0.0f,TransformedView_ScreenWorldY(&tv,GetHeight()) };
	Vec2 whls = { TransformedView_ScreenWorldX(&tv,0.0f),0.0f };
	Vec2 whle = { TransformedView_ScreenWorldX(&tv,GetWidth()),0.0f };

	Vec2 svls = TransformedView_WorldScreenPos(&tv,wvls);
	Vec2 svle = TransformedView_WorldScreenPos(&tv,wvle);
	Vec2 shls = TransformedView_WorldScreenPos(&tv,whls);
	Vec2 shle = TransformedView_WorldScreenPos(&tv,whle);

	RenderLine(svls,svle,WHITE,1.0f);
	RenderLine(shls,shle,WHITE,1.0f);


	const Vec2 len = TransformedView_ScreenWorldLength(&tv,(Vec2){ GetWidth(),GetHeight() });
	const float vrange = Vec2_Mag(len);

	const Vec2 PosBefore_Out = Function_2D(-vrange);
	Vec2 PosBefore = TransformedView_WorldScreenPos(&tv,(Vec2){ PosBefore_Out.x,-PosBefore_Out.y });
	const float step = vrange / 100.0f;

	for(float lambda = -vrange + step;lambda<vrange;lambda+=step){
		const Vec2 Out = Function_2D(lambda);
		const Vec2 Pos = TransformedView_WorldScreenPos(&tv,(Vec2){ Out.x,-Out.y });
		
		RenderLine(PosBefore,Pos,RED,1.0f);
		PosBefore = Pos;
	}

	Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,0.0f,WHITE,"P: X: %f, Y: %f",p.x,p.y);
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,GetAlxFont()->CharSizeY,WHITE,"Parameter: %f",parameter);
}

void Delete(AlxWindow* w){
	
}

int main(){
    if(Create("Geogebra Vec2",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}