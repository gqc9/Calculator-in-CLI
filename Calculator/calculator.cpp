#include <iostream>
#include <string>
#include <stack>
#include <vector>
using namespace std;

bool priorTo(char now, char top) {	// 当前now优先级高于栈顶top，返回true
	if ((now=='*' || now=='/') && (top=='+' || top=='-') && top!='(') {
		return true;
	}
	return false;
}

string operate(string first, string second, string op) { // first [oper] second
	double res = 0;
	double fir = stof(first);
	double sec = stof(second);
	switch (op[0]) {
		case '+': res = fir+sec; break;
		case '-': res = fir-sec; break;
		case '*': res = fir*sec; break;
		case '/': {
			if (sec==0) {
				cerr << "cannot divide by zero" << endl;
				exit(1);
			}
			res = fir/sec; 
			break;
		}
		default:  cerr << "not formula: operator error" << endl; exit(1);
	}
	
	return to_string(res);
}

// 前缀表达式->后缀表达式（RPN）
vector<string> toRPN(string expr) {
	stack<char> op;	// 存储运算符
	vector<string> rpn;

	for (int i = 0; i < expr.size(); i++) {		
		if (isdigit(expr[i])) { // 数字，合并小数
			string num = "";
			num += expr[i++];
			while (isdigit(expr[i]) || expr[i] == '.') {
				num += expr[i++];
			}
			rpn.push_back(num);
		}		
		// 符号
		if (expr[i] == ')') { 
			while (op.top() != '(') {
				if (op.empty()) {
					cerr << "not formula: bracket error" << endl;
					exit(1);
				}
				string tmp="";
				rpn.push_back(tmp + op.top());
				op.pop();
			}
			op.pop();
		}
		else if (op.empty() || op.top()=='(' || expr[i]=='(') {
			op.push(expr[i]);
		}
		else if (priorTo(expr[i], op.top())) { // 栈顶（+）优先级低于当前（*）
			op.push(expr[i]);
		}
		else { // 栈中元素出栈，当前进栈
			while (!op.empty() && op.top()!='(' && !priorTo(expr[i], op.top())) {
				string tmp = "";
				rpn.push_back(tmp + op.top());
				op.pop();
			}
			op.push(expr[i]);
		}
	}
	// 弹出留在栈内的运算符
	while (!op.empty()) {
		if ((int)op.top()==0) {
			op.pop();
			continue;
		}
		if (op.top()=='(') {
			cerr << "not formula" << endl;
			exit(1);
		}
		string tmp = "";
		tmp += op.top();
		rpn.push_back(tmp);
		op.pop();
	}
	
	for (int i = 0; i<rpn.size(); i++) {
		cout << rpn.at(i) << ' ';
	}
	cout<<endl;

	return rpn;
}


// 计算后缀表达式
double calculateRPN(vector<string> rpn) {
	stack<string> nums;
	for (int i = 0; i<rpn.size(); i++) {
		string element = rpn.at(i);		
		if (element==" ") {
			continue;
		}
		if (isdigit(element[0])) {	// 是数字，直接进栈
			nums.push(element);
		}		
		else if (nums.size()<2) {
			cerr << "not formula" << endl;
			exit(1);
		}
		else {	// 是符号，栈顶两个数字出栈，运算，运算结果进栈
			string second = nums.top();	// first [oper] second
			nums.pop();
			string first = nums.top();
			nums.pop();
			string result = operate(first, second, element);
			nums.push(result);
		}
	}
	if (nums.size()!=1) {
		cerr << "not formula" << endl;
		exit(1);
	}
	string res = nums.top();
	return stof(res);
}


int main() {
	while (1) {
		cout << "enter expression:" << endl;	
		string expr;
		cin >> expr;
		vector<string> rpn = toRPN(expr);
		double result = calculateRPN(rpn);
		cout << "result=" << result << endl;
		cout << endl;
	}

	return 0;
}