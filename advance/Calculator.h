#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class Calculator {
public:
	Calculator(string input) {
		Equation = "(" + input + ")";
	}
	double start() {
		Remove_space(Equation);
		find_oprators(Equation, Equation.length());
		numbers=&find_numbers(Equation, operator_location, number_of_operators);
		return prosses(operator_char, numbers, number_of_operators, number_of_numbers);
	}
private:
	string Equation;					//معادله ورودی توسط کاربر
	char* operator_char;				//عملگرد های ریاضی وارد شده در معادله
	double *numbers;					//اعداد وارد شده در معادله
	int number_of_operators;			//تعداد عملگرد ها
	int number_of_numbers;				//تعداد اعداد
	int* keys;							//آرایه ای که نشان می دهد بعد عملگرد چندم عدد موجود است
	int* operator_location;
	//حذف فاصله های وارد شده توسط کاربر
	void Remove_space(string& input)
	{
		string input_ = "";
		for (int i = 0; input[i] != '\0'; i++)
			if (input[i] != ' ')
				input_ += input[i];
		input = input_;
	}
	//پیدا کردن عمگرد های ریاضی
	void find_oprators(string& input, int size) {
		vector<char> temp_char;
		vector<int> temp_location;
		for (int i = 0; i < size; i++)
			if (input[i] == '+' || input[i] == '-' || input[i] == '(' || input[i] == ')' || input[i] == '/' || input[i] == '*' || input[i] == '^' || input[i] == '%' || input[i] == '!')
			{
				temp_char.push_back(input[i]);
				temp_location.push_back(i);
			}
		int p = temp_char.size();
		operator_char = new char[p + 1];
		operator_location = new int[p + 1];
		int i;
		for (i = 0; i < p; i++) {
			operator_location[i] = temp_location[i];
			operator_char[i] = temp_char[i];
		}
		number_of_operators = i;
		operator_char[i] = '\0';
		operator_location[i] = '\0';
	}
	//پیدا کردن اعداد با فرض قرار داشتن اعداد بین عملگرد ها
	double& find_numbers(string& input, int* operator_location, int size)
	{
		double* out;
		int* loc;
		string temp;
		vector<double> f;
		vector<int> I;
		int i, q;
		for (i = 0; i < size; i++)
		{
			temp = input.substr(operator_location[i] + 1, operator_location[i + 1] - operator_location[i] - 1);
			if (temp.length() > 0) {
				I.push_back(i);
				f.push_back(stof(temp));
			}
		}
		out = new double[i];
		loc = new int[i];
		for (i = 0, q = f.size(); i < q; i++)
		{
			out[i] = f[i];
			loc[i] = I[i];
		}
		out[i] = '\0';
		loc[i] = '\0';
		keys = loc;
		number_of_numbers = i;
		return *out;
	}
	//پیدا کردن یه مقدار در ارایه و برگرداندن شماره خونه آن عضو ارایه
	int finder(int* list,int size, int value) {
		for (int i = 0; i<size; i++)
			if (list[i] == value)
				return i;
	}
	//حذف عملگردی که استفاده شده
	char* op_remover(char* T,int size,int start,int end)
	{
		int i;
		char* Y = new char[size-(end-start+1)];
		Y[size - (end - start + 1)] = '\0';
		for (i = 0; i < start ; i++) {
			Y[i] = T[i];
		}
		for (int j=end+1; j < size;j++, i++) {
			Y[i] = T[j];
		}
		return Y;
	}
	//حذف اعداد استفاده شده
	double* num_replacer(double* list_num, int size, int location,double value) {
		int i;
		double *Y= new double[size - 1];
		Y[size - 1] = '\0';
		for (i = 0; i < location; i++) {
			Y[i] = list_num[i];
		}
		Y[location] = value;
		for (int j = location+2; j < size ; j++) {
			i++;
			Y[i] = list_num[j];
		}
		return Y;
	}
	//تغیر ترتیب key
	int* key_remaker(int* list, int size, int location,int reduce) {
		int i;
		int  *Y= new int[size - 1];
		Y[size - 1] = '\0';
		for (i = 0; i < location; i++) {
			Y[i] = list[i];
		}
		for (i = location; i < size-1; i++) {
			Y[i] = list[i+1]-reduce;
		}

		return Y;
	}
	//پردازش اصلی
	double prosses(char* operator_char, double* numbers, int number_of_operators, int number_of_numbers)
	{
		int F = 0,loc;
		int start_operator_location{};
		int end_operator_location{};
		int z;
		double result;
a:		for (size_t i = 0; i < operator_char[0] != '\0'; i++)
		{
			for (size_t i = number_of_operators; i >= 0; i--)
			{
				if (operator_char[i] == ')') {
					end_operator_location = i;
				}
				if (operator_char[i] == '(')
				{
					start_operator_location = i;
					break;
				}
			}
			F = end_operator_location - start_operator_location-1;
			if (F==0)
			{
				operator_char=op_remover(operator_char, number_of_operators, start_operator_location, end_operator_location);
				loc = finder(keys, number_of_operators,start_operator_location);
				number_of_operators -= 2;
				int i;
				int* Y = new int[number_of_numbers];
				Y[number_of_numbers] = '\0';
				for (i = 0; i <loc; i++) {
					Y[i] =keys[i];
				}
				Y[i] = keys[loc] - 1;
				for (i = loc+1; i < number_of_numbers; i++) {
					Y[i] = keys[i] - 2;
				}
				keys = Y;
			}
			if (F>0){
				for (z = end_operator_location ; z > start_operator_location; z--) {
					if (operator_char[z] == '^') {
						loc = finder(keys,number_of_numbers ,z);
						result = pow(numbers[loc-1], numbers[loc]);
						keys=key_remaker(keys, number_of_numbers,loc, 1);
						operator_char=op_remover(operator_char, number_of_operators, z, z);
						number_of_operators -= 1;
						numbers = num_replacer(numbers, number_of_numbers, loc - 1,result);
						number_of_numbers -= 1;
					goto a;
					}
				}
				for (z = end_operator_location; z > start_operator_location; z--) {
					if (operator_char[z] == '*') {
						loc = finder(keys, number_of_numbers, z);
						result =(double) (numbers[loc - 1]* numbers[loc]);
						keys = key_remaker(keys, number_of_numbers, loc, 1);
						operator_char = op_remover(operator_char, number_of_operators, z, z);
						number_of_operators -= 1;
						numbers = num_replacer(numbers, number_of_numbers, loc - 1, result);
						number_of_numbers -= 1;
					goto a;
					}
				}
				for (z = end_operator_location; z > start_operator_location; z--) {
					if (operator_char[z] == '/') {
						loc = finder(keys, number_of_numbers, z);
						result = (double)(numbers[loc - 1] / numbers[loc]);
						keys = key_remaker(keys, number_of_numbers, loc, 1);
						operator_char = op_remover(operator_char, number_of_operators, z, z);
						number_of_operators -= 1;
						numbers = num_replacer(numbers, number_of_numbers, loc - 1, result);
						number_of_numbers -= 1;
					goto a;
						
					}
				}
				for (z = end_operator_location; z > start_operator_location; z--) {
					if (operator_char[z] == '+') {
						loc = finder(keys, number_of_numbers, z);
						result = (double)(numbers[loc - 1] + numbers[loc]);
						keys = key_remaker(keys, number_of_numbers, loc, 1);
						operator_char = op_remover(operator_char, number_of_operators, z, z);
						number_of_operators -= 1;
						numbers = num_replacer(numbers, number_of_numbers, loc - 1, result);
						number_of_numbers -= 1;
					goto a;
					}
				}
				for (z = end_operator_location; z > start_operator_location; z--) {
					if (operator_char[z] == '-') {
						loc = finder(keys, number_of_numbers, z);
						result = (double)(numbers[loc - 1] - numbers[loc]);
						keys = key_remaker(keys, number_of_numbers, loc, 1);
						operator_char = op_remover(operator_char, number_of_operators, z, z);
						number_of_operators -= 1;
						numbers = num_replacer(numbers, number_of_numbers, loc - 1, result);
						number_of_numbers -= 1;
					goto a;
					}
				}
			}
		}
		return numbers[0];
	}
};