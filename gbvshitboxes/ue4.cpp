#include "ue4.h"
#include "sigscan.h"
#include <windows.h>

const EngineMemAlign_t EngineMemAlign = (EngineMemAlign_t)get_rip_relative(
	sigscan::get().scan("\x33\xD2\x8D\x4A\x14\xE8", "xxxxxx") + 6);

const EngineFree_t EngineFree = (EngineFree_t)(
	sigscan::get().scan("\x60\x8B\x44\x24\x30\x41\x89\x06", "xxxxxxxx") + 13);

using FCanvasTriangleItem_ctor_t = void(*)(
	FCanvasTriangleItem*,
	const FVector2D&,
	const FVector2D&,
	const FVector2D&,
	const FTexture*);
const auto FCanvasTriangleItem_ctor = (FCanvasTriangleItem_ctor_t)(
	sigscan::get().scan("\x94\xBD\xC4\x00\x48\x89\x01\x48\x8D\x05\xB2", "xxxxxxxxxxx") - 0x20);

using FCanvas_Flush_GameThread_t = void(*)(FCanvas *, bool);
const auto FCanvas_Flush_GameThread = (FCanvas_Flush_GameThread_t)(
	sigscan::get().scan("\x0D\x83\x89\x2D\x02\x48\x89\xB4", "xxxxxxxx") - 0x30);

using UCanvas_K2_DrawLine_t = void(*)(UCanvas*, FVector2D, FVector2D, float, const FLinearColor&);
const auto UCanvas_K2_DrawLine = (UCanvas_K2_DrawLine_t)(
	sigscan::get().scan("\x28\xF3\x0F\x5C\xC5\x41\x0F\x29\x7B\xD8", "xxxxxxxxxx") - 0x30);

using UCanvas_K2_Project_t = void(*)(const UCanvas*, FVector*, const FVector&);
const auto UCanvas_K2_Project = (UCanvas_K2_Project_t)(
	sigscan::get().scan("\x48\x83\xEC\x30\xF2\x41\x0F\x10\x00\x48\x8B\xDA", "xxxxxxxxxxxx") - 2);

using UCanvas_K2_DrawText_t = void(*)(
	UCanvas*,
	UFont*,
	const FString&,
	FVector2D,
	const FLinearColor&,
	float,
	const FLinearColor&,
	FVector2D,
	bool,
	bool,
	bool,
	const FLinearColor&);
UCanvas_K2_DrawText_t UCanvas_K2_DrawText;

using UCanvas_K2_DrawTexture_t = void(*)(
	UCanvas*,
	UTexture*,
	FVector2D,
	FVector2D,
	FVector2D,
	FVector2D,
	const FLinearColor&,
	EBlendMode,
	float,
	FVector2D);
const auto UCanvas_K2_DrawTexture = (UCanvas_K2_DrawTexture_t)(
	sigscan::get().scan("\x89\x4C\x24\x20\x0F\x57\xED\xF3", "xxxxxxxx") - 0x10);

using UCanvas_K2_DrawTriangle_t = void(*)(UCanvas*, UTexture*, TArray<FCanvasUVTri>*);
const auto UCanvas_K2_DrawTriangle = (UCanvas_K2_DrawTriangle_t)(
	sigscan::get().scan("\x48\x81\xEC\xA0\x00\x00\x00\x41\x83\x78\x08\x00", "xxxxxxxxxxxx") - 6);

HMODULE BaseModule = GetModuleHandleW(NULL);

//UWorld** GWorld = reinterpret_cast<UWorld**>(BaseModule + 0x3EFAA48);

UWorld **GWorld = (UWorld**)get_rip_relative(
	sigscan::get().scan("\x48\x8B\x1D\x86\x87\xC5\x03", "xxxxxxx") + 0x3);

FTexture **GWhiteTexture = (FTexture**)get_rip_relative((uintptr_t)UCanvas_K2_DrawTriangle + 0x3A);

FCanvasTriangleItem::FCanvasTriangleItem(
	const FVector2D &InPointA,
	const FVector2D &InPointB,
	const FVector2D &InPointC,
	const FTexture *InTexture)
{
	FCanvasTriangleItem_ctor(this, InPointA, InPointB, InPointC, InTexture);
}

void FCanvas::Flush_GameThread(bool bForce)
{
	FCanvas_Flush_GameThread(this, bForce);
}

void FCanvas::DrawItem(FCanvasItem &Item)
{
	Item.Draw(this);

	if (DrawMode == CDM_ImmediateDrawing)
		Flush_GameThread();
}

bool UObject::IsA(UClass* cmp) const
{
	for (auto super = Class; super; super = static_cast<UClass*>(super->SuperField))
	{
		if (super == cmp)
			return true;
	}

	return false;
}


void UCanvas::K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, float Thickness, const FLinearColor &RenderColor)
{
	UCanvas_K2_DrawLine(this, ScreenPositionA, ScreenPositionB, Thickness, RenderColor);
}

FVector UCanvas::K2_Project(const FVector &WorldPosition) const
{
	FVector out;
	UCanvas_K2_Project(this, &out, WorldPosition);
	return out;
}

void UCanvas::K2_DrawText(
	UFont *RenderFont,
	const FString &RenderText,
	FVector2D ScreenPosition,
	const FLinearColor &RenderColor,
	float Kerning,
	const FLinearColor &ShadowColor,
	FVector2D ShadowOffset,
	bool bCentreX,
	bool bCentreY,
	bool bOutlined,
	const FLinearColor &OutlineColor)
{
	UCanvas_K2_DrawText(
		this,
		RenderFont,
		RenderText,
		ScreenPosition,
		RenderColor,
		Kerning,
		ShadowColor,
		ShadowOffset,
		bCentreX,
		bCentreY,
		bOutlined,
		OutlineColor);
}

void UCanvas::K2_DrawTexture(
	UTexture *RenderTexture,
	FVector2D ScreenPosition,
	FVector2D ScreenSize,
	FVector2D CoordinatePosition,
	FVector2D CoordinateSize,
	const FLinearColor &RenderColor,
	EBlendMode BlendMode,
	float Rotation,
	FVector2D PivotPoint)
{
	UCanvas_K2_DrawTexture(
		this,
		RenderTexture,
		ScreenPosition,
		ScreenSize,
		CoordinatePosition,
		CoordinateSize,
		RenderColor,
		BlendMode,
		Rotation,
		PivotPoint);
}

void UCanvas::K2_DrawTriangle(UTexture *RenderTexture, TArray<FCanvasUVTri> *Triangles)
{
	UCanvas_K2_DrawTriangle(this, RenderTexture, Triangles);
}
