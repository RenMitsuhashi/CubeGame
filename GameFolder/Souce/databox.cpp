#include "databox.h"
#include "game.h"

DATA g_Data;

void Initdata(void)
{
	g_Data.nNowStage = STAGE_TUTORIAL;
	g_Data.nMiss = 0;
	g_Data.nScore = 0;
	g_Data.nTime = 0;
}

DATA *Getdata(void)
{
	return &g_Data;
}