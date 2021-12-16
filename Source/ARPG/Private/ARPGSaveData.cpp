
#include "ARPGSaveData.h"

void UARPGSaveData::AddNewRecord(FString Name,float Score)
{
	FSaveKey NewKey;
	NewKey.Score = Score;
	NewKey.Name = Name;
	Scores.Add(NewKey);
	Scores.Sort([](const FSaveKey A,const FSaveKey B){return A.Score > B.Score;});
	if(Scores.Num() >= MaxSaveCount)
	{
		Scores.RemoveAt(Scores.Num() - 1);
	}
}