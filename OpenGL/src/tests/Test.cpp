#include "Test.h"

#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currenttestpointer)
		:m_CurrentTest(currenttestpointer)
	{
	}

	void TestMenu::OnIMGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}