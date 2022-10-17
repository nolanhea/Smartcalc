#include <math.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <stack>
#include <string>

namespace s21 {
class SmartcalcModel {
 private:
  std::string _expr_str;
  double _x;
  struct token {
    double value;
    char type;
    token(double val, char t) : value(val), type(t) {}
  };
  std::queue<token> _input_queue;
  std::queue<token> _output_queue;
  std::stack<token> _operators_stack;
  bool _valid;

 public:
  SmartcalcModel(std::string expr, double x = NAN)
      : _expr_str(expr), _x(x), _valid(1) {}
  SmartcalcModel() : _expr_str(""), _x(NAN), _valid(0) {}
  void setModel(std::string expr, double x = NAN) {
    _expr_str = expr;
    _x = x;
    _valid = 1;
  }
  void resetModel() {
    while (!_input_queue.empty()) {
      _input_queue.pop();
    }
    while (!_output_queue.empty()) {
      _output_queue.pop();
    }
    while (!_operators_stack.empty()) {
      _operators_stack.pop();
    }
  }
  bool getValid() { return _valid; }
  double evaluate() {
    std::string str_operators = "+-*/^m";
    std::string str_other = "cstCSTrlLpd";
    if (!_input_queue.empty() || !_operators_stack.empty()) {
      _valid = 0;
      return 0;
    }
    while (!_output_queue.empty()) {
      token t = _output_queue.front();

      _output_queue.pop();

      if (t.type == 'n') {
        _operators_stack.push(t);
      } else if (str_operators.find(t.type) != std::string::npos) {
        token first(NAN, 'e');
        token second(NAN, 'e');
        if (_operators_stack.size() > 1) {
          first = _operators_stack.top();
          _operators_stack.pop();
          second = _operators_stack.top();
          _operators_stack.pop();
        }
        if (first.type != 'n' || second.type != 'n') {
          _valid = 0;
        } else {
          double result = 0;
          _valid = math_2(first.value, second.value, &result, t.type);
          token t_new(result, 'n');
          _operators_stack.push(t_new);
        }
      } else if (str_other.find(t.type) != std::string::npos) {
        token first(NAN, 'e');
        if (_operators_stack.size() >= 1) {
          first = _operators_stack.top();
          _operators_stack.pop();
        } else {
          _valid = 0;
        }
        if (first.type != 'n') {
          _valid = 0;
        } else {
          double result = 0;

          _valid = math_1(first.value, &result, t.type);
          token topush(result, 'n');
          _operators_stack.push(topush);
        }
      } else {
        _valid = 0;
      }
      if (!_valid) {
        break;
      }
    }
    if (_operators_stack.size() != 1) {
      _valid = 0;
      return 0;
    }
    double result = _operators_stack.top().value;
    _operators_stack.pop();
    return result;
  }
  double getResult() {
    double result = 0;
    if (_valid) {
      this->exprToTokensQueue();
    };
    if (_valid) {
      this->convertInfixToRpn();
    };
    if (_valid) {
      result = this->evaluate();
    };
    return result;
  }

 private:
  void showQueue() {
    while (!_output_queue.empty()) {
      token t = _output_queue.front();
      std::cout << t.type << ' ' << t.value << std::endl;
      _output_queue.pop();
    }
  }
  int math_1(double value, double *result, char type) {
    int ret = 1;
    if (type == 'c') {
      *result = cos(value);
    } else if (type == 's') {
      *result = sin(value);
    } else if (type == 't') {
      *result = tan(value);
    } else if (type == 'C') {
      *result = acos(value);
    } else if (type == 'S') {
      *result = asin(value);
    } else if (type == 'T') {
      *result = atan(value);
    } else if (type == 'r' && value >= 0) {
      *result = sqrt(value);
    } else if (type == 'l' && value > 0) {
      *result = log(value);
    } else if (type == 'L' && value > 0) {
      *result = log10(value);
    } else if (type == 'p') {
      *result = value;
    } else if (type == 'd') {
      *result = value * -1;
    } else {
      ret = 0;
    }
    return ret;
  }

  int math_2(double value2, double value, double *result, char type) {
    int ret = 1;
    if (type == '+') {
      *result = value + value2;
    } else if (type == '-') {
      *result = value - value2;
    } else if (type == '*') {
      *result = value * value2;
    } else if (type == '/' && value2 != 0) {
      *result = value / value2;
    } else if (type == '^') {
      *result = pow(value, value2);
    } else if (type == 'm' && value2 != 0) {
      *result = fmod(value, value2);
    } else {
      ret = 0;
    }
    return ret;
  }
  size_t pushFunctionToQueue(int pos) {
    size_t ret = 0;
    if (_expr_str.compare(pos, 3, "mod") == 0) {
      token t(NAN, 'm');
      _input_queue.push(t);
      ret = 3;
    } else if (_expr_str.compare(pos, 3, "cos") == 0) {
      token t(NAN, 'c');
      _input_queue.push(t);
      ret = 3;
    } else if (_expr_str.compare(pos, 3, "sin") == 0) {
      token t(NAN, 's');
      ret = 3;
      _input_queue.push(t);
    } else if (_expr_str.compare(pos, 3, "tan") == 0) {
      token t(NAN, 't');
      _input_queue.push(t);
      ret = 3;
    } else if (_expr_str.compare(pos, 4, "acos") == 0) {
      token t(NAN, 'C');
      _input_queue.push(t);
      ret = 4;
    } else if (_expr_str.compare(pos, 4, "asin") == 0) {
      token t(NAN, 'S');
      _input_queue.push(t);
      ret = 4;
    } else if (_expr_str.compare(pos, 4, "atan") == 0) {
      token t(NAN, 'T');
      _input_queue.push(t);
      ret = 4;
    } else if (_expr_str.compare(pos, 4, "sqrt") == 0) {
      token t(NAN, 'r');
      _input_queue.push(t);
      ret = 4;
    } else if (_expr_str.compare(pos, 2, "ln") == 0) {
      token t(NAN, 'l');
      _input_queue.push(t);
      ret = 2;
    } else if (_expr_str.compare(pos, 3, "log") == 0) {
      token t(NAN, 'L');
      _input_queue.push(t);
      ret = 3;
    }
    return ret;
  }

  void exprToTokensQueue() {
    _valid = 1;
    size_t pos = 0;
    if (_expr_str.size() == 0) {
      _valid = 0;
    }
    while (pos < _expr_str.size() && _valid) {
      std::string numbers_charackters = "1234567890.eE";
      std::string functions_charackters = "modcsintaqrlg";
      std::string operators_charackters = "()+-*/^pd";
      if (numbers_charackters.find(_expr_str[pos]) != std::string::npos) {
        size_t number_length = 0;
        if (_expr_str.substr(pos).find_first_of("1234567890") ==
            std::string::npos) {
          _valid = 0;
          return;
        }
        double number = stod(_expr_str.substr(pos), &number_length);
        token num(number, 'n');
        _input_queue.push(num);
        pos += number_length;
      } else if (functions_charackters.find(_expr_str[pos]) !=
                 std::string::npos) {
        size_t isValid = pushFunctionToQueue(pos);
        if (!isValid) {
          _valid = 0;
        } else {
          pos += isValid;
        }
      } else if (operators_charackters.find(_expr_str[pos]) !=
                 std::string::npos) {
        std::string unary_case = "(+*-/^mEdp";
        if (_expr_str[pos] == '+' &&
            (_input_queue.empty() ||
             unary_case.find(_input_queue.back().type) != std::string::npos)) {
          _expr_str[pos] = 'p';
        }
        if (_expr_str[pos] == '-' &&
            (_input_queue.empty() ||
             unary_case.find(_input_queue.back().type) != std::string::npos)) {
          _expr_str[pos] = 'd';
        }
        token op(NAN, _expr_str[pos]);
        _input_queue.push(op);
        pos++;
      } else if (_expr_str[pos] == ' ') {
        pos++;
      } else if (_expr_str[pos] == 'x') {
        if (isnan(_x)) {
          _valid = 0;
        } else {
          token num(_x, 'n');
          _input_queue.push(num);
          pos++;
        }
      } else {
        _valid = 0;
      }
    }
  }
  int getPrecedence(char op) {
    int precedence = 0;
    if (op == '+' || op == '-') {
      precedence = 1;
    } else if (op == '*' || op == '/' || op == 'm') {
      precedence = 2;
    } else if (op == '^') {
      precedence = 3;
    } else if (op == 'd' || op == 'p') {
      precedence = 4;
    }
    return precedence;
  }
  void convertInfixToRpn() {
    while (!_input_queue.empty()) {
      token t = _input_queue.front();
      _input_queue.pop();
      std::string case_1 = "(cstCSTrlL";
      std::string case_2 = "+-*/^mpd";
      std::string case_3 = "^dp";
      std::string case_4 = "cstCSTrlL";
      if (t.type == 'n') {
        _output_queue.push(t);
      } else if (case_1.find(t.type) != std::string::npos) {
        _operators_stack.push(t);
      } else if (case_2.find(t.type) != std::string::npos) {
        int precedence_input = 0;
        int precedence_stack = 0;
        if (!_operators_stack.empty()) {
          precedence_stack = this->getPrecedence(_operators_stack.top().type);
        }
        precedence_input = getPrecedence(t.type);
        while (!_operators_stack.empty() &&
               ((precedence_stack > precedence_input) ||
                (precedence_stack == precedence_input &&
                 case_3.find(t.type) == std::string::npos))) {
          token op = _operators_stack.top();
          _operators_stack.pop();
          if (!_operators_stack.empty()) {
            precedence_stack = this->getPrecedence(_operators_stack.top().type);
          }
          _output_queue.push(op);
        }
        _operators_stack.push(t);
      } else if (t.type == ')') {
        while (!_operators_stack.empty() &&
               _operators_stack.top().type != '(') {
          token op = _operators_stack.top();
          _operators_stack.pop();
          _output_queue.push(op);
        }
        if (!_operators_stack.empty()) {
          _operators_stack.pop();
          if (!_operators_stack.empty() &&
              case_4.find(_operators_stack.top().type) != std::string::npos) {
            token op = _operators_stack.top();
            _operators_stack.pop();
            _output_queue.push(op);
          }
        } else {
          _valid = 0;
        }
      } else if (!_input_queue.empty()) {
        t = _input_queue.front();
        _input_queue.pop();
      }
    }
    while (!_operators_stack.empty()) {
      token op = _operators_stack.top();
      _operators_stack.pop();
      if (op.type == '(' || op.type == ')') {
        _valid = 0;
      }
      _output_queue.push(op);
    }
  }
};
}  // namespace s21
