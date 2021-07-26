#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>

namespace test {
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltatime) {}
		virtual void OnRender() {}
		virtual void OnIMGuiRender() {}
	};

	class TestMenu: public Test
	{
	public:
		TestMenu(Test*& currenttestpointer);

		void OnIMGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test" << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}