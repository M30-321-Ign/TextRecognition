#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Control_list2/Header.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace list_test
{		
	TEST_CLASS(TestSQL)
	{
	public:
		
		TEST_METHOD(TestCreate)
		{
			_sql C;
			Assert::IsTrue(C._sqlCreateTable() == 0);
			Assert::IsTrue(C.CreateUser() == 0);
			
		}
		TEST_METHOD(TestSQL_f2time) {
		
			float mas1[10] = {76.0,7.09,9.78,6.7,4.56,111.2,65.2,45.2,7.0,6.06};
			string mas2[10] = { "--:--","07:09","--:--","--:--","04:56","--:--","--:--","--:--","07:00","06:06" };
			_sql C;
			for (int i = 0; i < 10; i++) {
				Assert::IsTrue(C.f2time(mas1[i]) == mas2[i]);
			}
		}
		TEST_METHOD(Test_true_name) {

			_sql C;
			Assert::IsTrue(C.true_name("nnn Игнатьев Андрей Александрович nn") == "Игнатьев Андрей Александрович");
			
		}

	};

	

}