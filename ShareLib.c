#include "ShareLib.h"
 
typedef int (*CalculatorFuncPointer)(int, int);
 
int ShareLib(void) 
{
	const char *dlib_path = "/var/fpwork/lyun/test/lib/libCal.so";
	//貌似只能是绝对路径，相对路径找不到文件

	void *handle = dlopen(dlib_path, RTLD_GLOBAL | RTLD_NOW);
	if (handle == NULL) 
	{
		fprintf(stderr, "%s\n", dlerror());
		return -1;
	} 
	else 
	{
		CalculatorFuncPointer add_func = (CalculatorFuncPointer)dlsym(handle, "add");
		int add_ret = add_func(10, 20);
		printf("add function result : %d \n", add_ret);

		CalculatorFuncPointer subtract_func = (CalculatorFuncPointer)dlsym(handle, "subtract");
		int subtract_ret = subtract_func(10, 20);
		printf("subtract function result : %d \n", subtract_ret);

		CalculatorFuncPointer multiply_func = (CalculatorFuncPointer)dlsym(handle, "multiply");
		int multiply_ret = multiply_func(10, 20);
		printf("multiply function result : %d \n", multiply_ret);

		CalculatorFuncPointer divide_func = (CalculatorFuncPointer)dlsym(handle, "divide");
		int divide_ret = divide_func(10, 2);
		printf("divide function result : %d \n", divide_ret);
		dlclose(handle);
	}
	return 0;
}
