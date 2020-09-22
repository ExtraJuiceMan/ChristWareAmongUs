#pragma once
#include <imgui.h>

class IterationCounter
{
	int iterations;
	int actionCount;
public:
	IterationCounter(int actionCount)
	{
		this->actionCount = actionCount;
		iterations = 0;
	}

	void SetActionCount(int count)
	{
		if (count != actionCount)
		{
			iterations = 0;
			actionCount = count;
		}
	}

	int GetActionCount()
	{
		return actionCount;
	}

	bool ProcessAction()
	{
		iterations++;

		if (iterations == actionCount)
		{
			iterations = 0;
			return true;
		}

		return false;
	}

	void GenerateInput(const char* inputName)
	{
		int interval = this->GetActionCount();
		ImGui::InputInt(inputName, &interval);
		if (interval != this->GetActionCount())
			this->SetActionCount(interval);
	}
};