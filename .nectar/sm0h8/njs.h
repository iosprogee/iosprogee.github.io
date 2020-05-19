 #include <iostream>
 #include <string>
 #include <cstring>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sstream>
 #include <memory>
 #include <vector>

#ifdef __NJS_ARDUINO
 #include <util/delay.h>
 #include <string.h>
 #include <WString.h>
 #include <new.h>
 #include <time.h>
 #include <nonstd-function.h>
	using namespace nonstd;
#else
	 #include <functional>
#endif
 
 using namespace std;

enum __NJS_TYPE
{
	__NJS_UNDEFINED = 1,
	__NJS_NUMBER,
	__NJS_DOUBLE,
	__NJS_OBJECT,
	__NJS_BIGNUMBER,
	__NJS_BOOLEAN,
	__NJS_STRING,
	//__NJS_NATIVE,
	__NJS_FUNCTION,
	__NJS_ARRAY,
	//__NJS_NAN,
	
};

/*** HELPERS ***/
#ifdef __NJS_ARDUINO
	#define __NJS_FFI_FUNCTION(_name, ...) function<__NJS_VAR (__VA_ARGS__)>  _name   = [](__VA_ARGS__) -> __NJS_VAR 
#else 
	#define __NJS_FFI_FUNCTION(_name, ...) function<__NJS_VAR (__VA_ARGS__)> _name   = [](__VA_ARGS__) -> __NJS_VAR
#endif
#define __NJS_GET_STRING(_var) _var.get().s->__NJS_VALUE.c_str()
#define var __NJS_VAR
#define let __NJS_VAR


#define __NJS_Create_Boolean(_value) __NJS_VAR(__NJS_BOOLEAN, _value)
#define __NJS_Create_Number(_value) __NJS_VAR( _value)
#define true __NJS_Create_Boolean(true)
#define false __NJS_Create_Boolean(false)
#define __NJS_Create_Undefined() __NJS_VAR()
#define __NJS_DECL_FUNCTION function
#define __NJS_RETURN_UNDEFINED var __undef = __NJS_VAR(); return __undef;

struct __NJS_VAR;


class __NJS_Class_Object
{
  public:
	int cnt = 0;
	__NJS_Class_Object();
    vector<pair<char*, __NJS_VAR>> __OBJECT;
    __NJS_VAR Get(char* _index);
	void Delete();
};

class __NJS_Class_String : public __NJS_Class_Object
{
  public:
  int cnt = 1;
    __NJS_Class_String(char* _str);
    __NJS_VAR Get(char* _index);
    string __NJS_VALUE;
};


class __NJS_Class_Array : public __NJS_Class_Object
{
  public:
    __NJS_Class_Array();
    __NJS_VAR Get(char* _index);
    vector<__NJS_VAR> __NJS_VALUE;
};

class __NJS_Class_Function : public __NJS_Class_Object
{
  public:
  __NJS_Class_Function(void* _f);
  //~__NJS_Class_Function();
    __NJS_VAR Get(char* _index);
    void* __NJS_VALUE;
	//void Delete();
};

union val
{
	int i;
	double d;
	bool b;
	__NJS_Class_String* s;
	__NJS_Class_Array* a;
	__NJS_Class_Object* o;
	__NJS_Class_Function* f;
};

/*** REGISTER ***/

int FREE_PTR = -1;
int REGISTER_PTR = 0;
val REGISTER[1000000]{(val){.i=0}};
int FREE[1000000] = {0};


/*** END REGISTER ***/

__NJS_VAR __create_Array(vector<__NJS_VAR>  _value);
__NJS_VAR __NJS_Log_Console(__NJS_VAR _var);
__NJS_VAR __NJS_Create_String(char* _value);
inline __NJS_VAR& __NJS_Object_Get(char* _index, __NJS_VAR _array);
inline __NJS_VAR& __NJS_Object_Get(int _index, __NJS_VAR _array);
inline __NJS_VAR& __NJS_Object_Get(__NJS_VAR& _index, __NJS_VAR _array);
inline __NJS_VAR __NJS_Object_Set(__NJS_VAR _index, __NJS_VAR _value, __NJS_VAR _object);


/*** STRDUP ***/
char* strdup (const char* s)
{
  size_t slen = strlen(s);
  char* result = (char*)malloc(slen + 1);
  if(result == NULL)
  {
    return NULL;
  }

  memcpy(result, s, slen+1);
  return result;
}

inline char* __NJS_Concat_Str_To_Int(int _left, char* _right)
{
	std::stringstream output;
	output << _left << _right;
	return strdup(output.str().c_str());
}

inline char* __NJS_Concat_Str_To_Int(double _left, char* _right)
{
	std::stringstream output;
	output << _left << _right;
	return strdup(output.str().c_str());
}

inline char* __NJS_Concat_Int_To_Str(char* _left, int _right)
{
	std::stringstream output;
	output << _left << _right;
	return strdup(output.str().c_str());
}

inline char* __NJS_Concat_Int_To_Str(char* _left, double _right)
{
	std::stringstream output;
	output << _left << _right;
	return strdup(output.str().c_str());
}

inline char* __NJS_Concat_Str_To_Str(const char* _left, const char* _right)
{
	char* _result = (char*)malloc(strlen(_left) + strlen(_right) + 1);
	strcpy(_result, _left);
	strcat(_result, _right);
	strcat(_result, "\0");
	return _result;
}



struct __NJS_VAR
{   	
	private:
	
		void setPtr()
		{
			if(_ptr > -1) 
			{
				return;
			}
			if(FREE_PTR > -1)
			{
				_ptr = FREE[FREE_PTR--];
			}
			else 
			{
				_ptr = REGISTER_PTR++;
			}
		}

	public:
		__NJS_TYPE type;
		int _ptr = -1;
	
		inline val get() const
		{
			return REGISTER[_ptr];
		}
	
		inline __NJS_VAR()
		{
		  setPtr();
		  type = __NJS_UNDEFINED;
		  REGISTER[_ptr].i = 0;
		}
		
		~__NJS_VAR()
		{	
			if(type ==  __NJS_OBJECT)
			{
				REGISTER[_ptr].o->Delete();
			}
			else if(type ==  __NJS_STRING)
			{
				REGISTER[_ptr].s->Delete();
			}
			else if(type ==  __NJS_FUNCTION)
			{
				REGISTER[_ptr].f->Delete();
			}
			else if(type ==  __NJS_ARRAY)
			{
				REGISTER[_ptr].a->Delete();
			}
			FREE[++FREE_PTR] = _ptr;
		}
		
		/**/
		__NJS_VAR(__NJS_VAR const& _v) 
		{ 
			setPtr();
			type = _v.type;
			
			if(_v.type == __NJS_OBJECT)
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
				REGISTER[_ptr].o->cnt++;
			}
			else if(_v.type == __NJS_STRING)
			{
				REGISTER[_v._ptr].s->cnt--;
				REGISTER[_ptr].s = new __NJS_Class_String((char*)REGISTER[_v._ptr].s->__NJS_VALUE.c_str());
			}
			else if(_v.type == __NJS_FUNCTION)
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
				REGISTER[_ptr].f->cnt++;
			}
			else if(_v.type == __NJS_ARRAY)
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
				REGISTER[_ptr].a->cnt++;
			}
			else
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
			}
			return;
		}
		/**/
		
		/*** CONSTRUCTOR ***/
		
		inline __NJS_VAR (__NJS_TYPE _type, int _value)
		{
			setPtr();
			this->type = _type;
			REGISTER[_ptr].i = _value;
		}
		
		inline __NJS_VAR (int _value)
		{
			setPtr();
			this->type = __NJS_NUMBER;
			REGISTER[_ptr].i = _value;
		}

		inline __NJS_VAR (double _value)
		{
			setPtr();
			this->type = __NJS_DOUBLE;
			REGISTER[_ptr].d = _value;
		}

		__NJS_VAR (char* _value)
		{
			setPtr();
			type = __NJS_STRING;
			REGISTER[_ptr].s = new __NJS_Class_String(_value);
		}
		
		__NJS_VAR (const char* _value)
		{
			setPtr();
			type = __NJS_STRING;
			REGISTER[_ptr].s = new __NJS_Class_String((char*)_value);
		}
		
		__NJS_VAR (__NJS_Class_String* _value)
		{
			setPtr();
			type = __NJS_STRING;
			REGISTER[_ptr].s = _value;
		}
		__NJS_VAR (__NJS_Class_Array* _value)
		{
			setPtr();
			type = __NJS_ARRAY;
			REGISTER[_ptr].a = _value;
		}
		__NJS_VAR (__NJS_Class_Object* _value)
		{
			setPtr();
			type = __NJS_OBJECT;
			REGISTER[_ptr].o = _value;
		}
		__NJS_VAR (__NJS_TYPE _type, void* _value)
		{
			setPtr();
			type = _type;
			REGISTER[_ptr].f = new __NJS_Class_Function(_value);
		}

		/*** VARIADIC LAMBDAS ***/
		template<class... Args>
		__NJS_VAR (function<__NJS_VAR (Args...)>& _value)
		{
			setPtr();
			type = __NJS_FUNCTION;
			REGISTER[_ptr].f = new __NJS_Class_Function(&_value);
		}
		/*** END VARIADIC LAMBDAS ***/
		

		/*** END CONSTRUCTOR ***/

		/*** OPERATOR ***/

		/*** EQUAL ***/
		__NJS_VAR& operator=(const __NJS_VAR& _v)
		{
			
			if(type == __NJS_OBJECT) REGISTER[_ptr].o->Delete();
			else if(type == __NJS_STRING) REGISTER[_ptr].s->Delete();
			else if(type == __NJS_FUNCTION) REGISTER[_ptr].f->Delete();
			else if(type == __NJS_ARRAY) REGISTER[_ptr].a->Delete();

			type = _v.type;;

			if(_v.type == __NJS_OBJECT)
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
				REGISTER[_ptr].o->cnt++;
			}
			else if(_v.type == __NJS_STRING)
			{
				REGISTER[_ptr].s = new __NJS_Class_String(strdup(REGISTER[_v._ptr].s->__NJS_VALUE.c_str()));
				REGISTER[_v._ptr].s->cnt--;
			}
			else if(_v.type == __NJS_FUNCTION)
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
				REGISTER[_ptr].f->cnt++;
			}
			else if(_v.type == __NJS_ARRAY)
			{
				REGISTER[_ptr] = REGISTER[_v._ptr];
				REGISTER[_ptr].a->cnt++;
			}
			else REGISTER[_ptr] = REGISTER[_v._ptr];
			
			return *this;
		}
		/*** END EQUAL ***/
		
		inline __NJS_VAR operator+(const __NJS_VAR& _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER)
			{
				return REGISTER[_ptr].i + REGISTER[_v1._ptr].i;
			}
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE)
			{
				return REGISTER[_ptr].i + REGISTER[_v1._ptr].d;
			}
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE)
			{
				return REGISTER[_ptr].d + REGISTER[_v1._ptr].d;
			}
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER)
			{
				return REGISTER[_ptr].d + REGISTER[_v1._ptr].i;
			}
			else if(type == __NJS_NUMBER && _v1.type == __NJS_STRING)
			{
				return __NJS_Create_String(__NJS_Concat_Str_To_Int(get().i, (char*)_v1.get().s->__NJS_VALUE.c_str()));
			}
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_STRING)
			{
				return __NJS_Create_String(__NJS_Concat_Str_To_Int(get().d, (char*)_v1.get().s->__NJS_VALUE.c_str()));
			}
			else if(type == __NJS_STRING && _v1.type == __NJS_NUMBER)
			{
				return __NJS_Create_String(__NJS_Concat_Int_To_Str((char*)get().s->__NJS_VALUE.c_str(), _v1.get().i));
			}
			else if(type == __NJS_STRING && _v1.type == __NJS_DOUBLE)
			{
				return __NJS_Create_String(__NJS_Concat_Int_To_Str((char*)get().s->__NJS_VALUE.c_str(), _v1.get().d));
			}
			else if(type == __NJS_STRING && _v1.type == __NJS_BOOLEAN)
			{
				if(_v1.get().b)
				{
				  return __NJS_Create_String(__NJS_Concat_Str_To_Str((char*)get().s->__NJS_VALUE.c_str(), (char*)"true"));
				}
				return __NJS_Create_String(__NJS_Concat_Str_To_Str((char*)get().s->__NJS_VALUE.c_str(), (char*)"false"));
			}
			else if(type == __NJS_STRING && _v1.type == __NJS_STRING)
			{
				return __NJS_Create_String(__NJS_Concat_Str_To_Str(get().s->__NJS_VALUE.c_str(), (char*)_v1.get().s->__NJS_VALUE.c_str()));
			}
			else
			{
				return __NJS_VAR(0);
			}
		}
		inline __NJS_VAR operator-(const __NJS_VAR& _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER) return __NJS_VAR( get().i - _v1.get().i );
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE) return __NJS_VAR( get().i - _v1.get().d );
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER) return __NJS_VAR( get().d - _v1.get().i );
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE) return __NJS_VAR( get().d - _v1.get().d );
			return __NJS_VAR();
		}

		inline __NJS_VAR operator-()
		{
			if(type == __NJS_NUMBER) return __NJS_VAR( -get().i);
			else if(type == __NJS_DOUBLE) return __NJS_VAR( -get().d);
			return __NJS_VAR();
		}

		inline __NJS_VAR operator+()
		{
			if(type == __NJS_NUMBER) return __NJS_VAR( get().i);
			else if(type == __NJS_DOUBLE) return __NJS_VAR( get().d);
			return __NJS_VAR();
		}


		__NJS_VAR operator+=(const __NJS_VAR _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER) REGISTER[_ptr].i += _v1.get().i;
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE) REGISTER[_ptr].i += _v1.get().d;
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER) REGISTER[_ptr].d += _v1.get().i;
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE) REGISTER[_ptr].d += _v1.get().d;
			return __NJS_VAR();
		}
	
   	 __NJS_VAR operator++(const int _v1)
		{
			if(type == __NJS_NUMBER) REGISTER[_ptr].i++;
			else if(type == __NJS_DOUBLE) REGISTER[_ptr].d++;
			return *this;
		}
    	__NJS_VAR operator--(const int _v1)
		{
			if(type == __NJS_NUMBER) REGISTER[_ptr].i--;
			else if(type == __NJS_DOUBLE) REGISTER[_ptr].d--;
			return *this;
		}

    	__NJS_VAR operator*(const __NJS_VAR& _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER) return __NJS_VAR( get().i * _v1.get().i );
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE) return __NJS_VAR( get().i * _v1.get().d );
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER) return __NJS_VAR( get().d * _v1.get().i );
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE) return __NJS_VAR( get().d * _v1.get().d );
			return __NJS_VAR();
		}
		__NJS_VAR operator*= (const __NJS_VAR& _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER) REGISTER[_ptr].i *= _v1.get().i;
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE) REGISTER[_ptr].i *= _v1.get().d;
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER) REGISTER[_ptr].d *= _v1.get().i;
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE) REGISTER[_ptr].d *= _v1.get().d;
			return __NJS_VAR();
		}
		__NJS_VAR operator/= (const __NJS_VAR& _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER) REGISTER[_ptr].i /= _v1.get().i;
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE) REGISTER[_ptr].i /= _v1.get().d;
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER) REGISTER[_ptr].d /= _v1.get().i;
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE) REGISTER[_ptr].d /= _v1.get().d;
			return __NJS_VAR();
		}
    __NJS_VAR operator/(const __NJS_VAR& _v1)
		{
			if(type == __NJS_NUMBER && _v1.type == __NJS_NUMBER) return __NJS_VAR( get().i / _v1.get().i );
			else if(type == __NJS_NUMBER && _v1.type == __NJS_DOUBLE) return __NJS_VAR( get().i / _v1.get().d );
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_NUMBER) return __NJS_VAR( get().d / _v1.get().i );
			else if(type == __NJS_DOUBLE && _v1.type == __NJS_DOUBLE) return __NJS_VAR( get().d / _v1.get().d );
			return __NJS_VAR();
		}
    __NJS_VAR operator%(const __NJS_VAR& _v1)
		{
			return __NJS_VAR( get().i % _v1.get().i );
		}
		
    __NJS_VAR const operator! () const
    {
      if(type == __NJS_NUMBER)
      {
        return __NJS_Create_Number(!get().i);
      }
      else return(false);
    };
		
    __NJS_VAR operator==(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR(__NJS_BOOLEAN, get().i == _v1.get().i);
				else if(type == __NJS_STRING)
				{
					if(strcmp(get().s->__NJS_VALUE.c_str(),_v1.get().s->__NJS_VALUE.c_str()) == 0) return true;
					else return false;
				}
			}
			return false;
		}
		
		__NJS_VAR operator<(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR(__NJS_BOOLEAN, get().i < _v1.get().i);
				else if(type == __NJS_STRING)
				{
					false;
				}
			}
			return false;
			
		}
		
    __NJS_VAR operator<=(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR(__NJS_BOOLEAN, get().i <= _v1.get().i);
				else if(type == __NJS_STRING)
				{
					false;
				}
			}
			return false;
		}

		__NJS_VAR operator>(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR(__NJS_BOOLEAN, get().i > _v1.get().i);
				else if(type == __NJS_STRING)
				{
					false;
				}
			}
			return false;
		}
		
    __NJS_VAR operator>=(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR(__NJS_BOOLEAN, get().i >= _v1.get().i);
				else if(type == __NJS_STRING)
				{
					false;
				}
			}
			return false;
		}
    __NJS_VAR operator>>(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR( get().i >> _v1.get().i);
			}
			return 0;
			
		}
    __NJS_VAR operator<<(const __NJS_VAR& _v1)
		{
			if(type == _v1.type )
			{
				if(type == __NJS_NUMBER) return __NJS_VAR( get().i << _v1.get().i);
			}
			return 0;
		}
	

	explicit operator bool() const 
	{ 
		if(type == __NJS_NUMBER || type == __NJS_BOOLEAN) return this->get().i; 
		else if(type == __NJS_DOUBLE) return this->get().d;
		else if(type == __NJS_UNDEFINED) return 0;
		else return 0;
		
	}
};

/*** VARIADIC CALLS ***/
template<class... Args>
inline __NJS_VAR __NJS_Back_Var_Call_Function(__NJS_VAR _obj, Args... args)
{
	return (*static_cast<function<__NJS_VAR ( Args... )>*>(_obj.get().f->__NJS_VALUE))( args... );
}

template<class... Args>
inline __NJS_VAR __NJS_Call_Function(__NJS_VAR _obj, Args... args)
{
	return __NJS_Back_Var_Call_Function(_obj, (__NJS_VAR)(args)...);
}
/*** END VARIADIC CALLS ***/


inline __NJS_VAR __create_Array()
{
  __NJS_Class_Array* _a = new __NJS_Class_Array();
  return __NJS_VAR(__NJS_ARRAY, _a);
}

inline __NJS_VAR __NJS_Create_Array()
{
  __NJS_Class_Array* _a = new __NJS_Class_Array();
  return __NJS_VAR(_a);
}

inline int __NJS_Get_Int(__NJS_VAR _v)
{
  return _v.get().i;
}

inline char* __NJS_Get_String(__NJS_VAR _v)
{
  return (char*)_v.get().s->__NJS_VALUE.c_str();
}

inline __NJS_VAR __NJS_Typeof(__NJS_VAR _var)
{
  char* _array[] = {(char*)"", (char*)"undefined", (char*)"number", (char*)"number", (char*)"object", (char*)"number", (char*)"boolean", (char*)"string", (char*)"function", (char*)"array"};
  return __NJS_Create_String(_array[_var.type]);
}

/*** ***/
inline __NJS_VAR __NJS_Object_Set(char* _index, __NJS_VAR _value, vector<pair<char*, __NJS_VAR>>* _obj)
{

  int _j = (*_obj).size();
  for(int _i = 0; _i < _j; _i++)
  {
    if(strcmp(_index, (*_obj)[_i].first) == 0)
    {
      (*_obj)[_i].second.type = _value.type;
	  
      REGISTER[(*_obj)[_i].second._ptr] = REGISTER[_value._ptr];
      
	  return __NJS_VAR();
    }
  }

  (*_obj).push_back(pair<char*, __NJS_VAR>( _index, _value));
  return __NJS_VAR();

}

inline __NJS_VAR __NJS_Object_Set(__NJS_VAR _index, __NJS_VAR _value, vector<pair<char*, __NJS_VAR>>* _obj)
{
  int _j = (*_obj).size();
  for(int _i = 0; _i < _j; _i++)
  {
    if(strcmp(_index.get().s->__NJS_VALUE.c_str(), (*_obj)[_i].first) == 0)
    {
      (*_obj)[_i].second.type = _value.type;
      
	  REGISTER[(*_obj)[_i].second._ptr] = REGISTER[_value._ptr];
      
	  return __NJS_VAR();
    }
  }

  (*_obj).push_back(pair<char*, __NJS_VAR>( (char*)_index.get().s->__NJS_VALUE.c_str(), _value));
  return __NJS_VAR();
}

inline __NJS_VAR __NJS_Object_Set(char* _index, __NJS_VAR _value, __NJS_VAR _array)
{
  vector<pair<char*, __NJS_VAR>>* _obj;
  if(_array.type == __NJS_OBJECT) _obj = &_array.get().o->__OBJECT;
  else if(_array.type == __NJS_ARRAY) _obj = &_array.get().a->__OBJECT;
  else if(_array.type == __NJS_STRING) _obj = &_array.get().s->__OBJECT;
  else if(_array.type == __NJS_FUNCTION) _obj = &_array.get().f->__OBJECT;
  else return __NJS_VAR();

  int _j = (*_obj).size();

  for(int _i = 0; _i < _j; _i++)
  {
    if(strcmp(_index, (*_obj)[_i].first) == 0)
    {
      (*_obj)[_i].second.type = _value.type;
	  
	  REGISTER[(*_obj)[_i].second._ptr] = REGISTER[_value._ptr];
	  
      return __NJS_VAR();
    }
  }

  (*_obj).push_back(pair<char*, __NJS_VAR>( _index, _value));
  return __NJS_VAR();
}

inline __NJS_VAR __NJS_Object_Set(__NJS_VAR _index, __NJS_VAR _value, __NJS_VAR _array)
{
  if(_array.type == __NJS_ARRAY && _index.type == __NJS_NUMBER)
  {
    if(_index.type != __NJS_NUMBER) return __NJS_VAR (__NJS_UNDEFINED, 0);

    if(_array.get().a->__NJS_VALUE.size() <= _index.get().i) _array.get().a->__NJS_VALUE.resize(_index.get().i + 1);

    _array.get().a->__NJS_VALUE[_index.get().i].type = _value.type;
    
	REGISTER[_array.get().a->__NJS_VALUE[_index.get().i]._ptr] = REGISTER[_value._ptr];

    __NJS_Object_Set(__NJS_Create_String((char*)"length"), __NJS_Create_Number((int)_array.get().a->__NJS_VALUE.size()), &_array.get().a->__OBJECT);
  }
  else if(_array.type == __NJS_OBJECT || _array.type == __NJS_STRING || _array.type == __NJS_FUNCTION || _array.type == __NJS_ARRAY)
  {
    vector<pair<char*, __NJS_VAR>>* _obj;
    if(_array.type == __NJS_OBJECT) _obj = &_array.get().o->__OBJECT;
	else if(_array.type == __NJS_ARRAY) _obj = &_array.get().a->__OBJECT;
    else if(_array.type == __NJS_STRING) _obj = &_array.get().s->__OBJECT;
	else if(_array.type == __NJS_FUNCTION) _obj = &_array.get().f->__OBJECT;
    else return __NJS_VAR();

    int _j = (*_obj).size();

    for(int _i = 0; _i < _j; _i++)
    {
      if(strcmp(_index.get().s->__NJS_VALUE.c_str(), (*_obj)[_i].first) == 0)
      {
        (*_obj)[_i].second.type = _value.type;
		
        REGISTER[(*_obj)[_i].second._ptr] = REGISTER[_value._ptr];
        return __NJS_VAR();
      }
    }
	
    (*_obj).push_back(pair<char*, __NJS_VAR>( (char*)_index.get().s->__NJS_VALUE.c_str(), _value));
  }
  return __NJS_VAR();
}

inline __NJS_VAR __NJS_Object_Set(int _index, __NJS_VAR _value, __NJS_VAR _array)
{
  if(_array.type == __NJS_ARRAY)
  {
    if(_array.get().a->__NJS_VALUE.size() <= _index) _array.get().a->__NJS_VALUE.resize(_index + 1);
    _array.get().a->__NJS_VALUE[_index].type = _value.type;

	if(_value.type == __NJS_STRING)
	{
		REGISTER[_array.get().a->__NJS_VALUE[_index]._ptr].s = new __NJS_Class_String(REGISTER[_value._ptr].s->__NJS_VALUE.c_str());
	}
	else REGISTER[_array.get().a->__NJS_VALUE[_index]._ptr] = REGISTER[_value._ptr];

	__NJS_Object_Set(__NJS_Create_String((char*)"length"), __NJS_VAR((int)_array.get().a->__NJS_VALUE.size()), _array);
  }
  return __NJS_VAR();
}


inline __NJS_VAR& __NJS_Object_Get(int _index, __NJS_VAR _array)
{
  if(_array.type != __NJS_ARRAY && _array.type != __NJS_OBJECT && _array.type != __NJS_FUNCTION)
  {
	  __NJS_RETURN_UNDEFINED;
  } 
  if(_array.type == __NJS_ARRAY)
  {
    if(_index > _array.get().a->__NJS_VALUE.size())
    {
      __NJS_RETURN_UNDEFINED;
    }
	 
    return _array.get().a->__NJS_VALUE[_index];
  }
  __NJS_RETURN_UNDEFINED;
}


inline __NJS_VAR& __NJS_Object_Get(char* _index, __NJS_VAR _array)
{
	if(_array.type != __NJS_OBJECT && _array.type != __NJS_STRING && _array.type != __NJS_FUNCTION && _array.type != __NJS_ARRAY) 
  {
	  __NJS_RETURN_UNDEFINED;
  }
  else
  {
    vector<pair<char*, __NJS_VAR>>* _obj;
    if(_array.type == __NJS_OBJECT) _obj = &_array.get().o->__OBJECT;
	else if(_array.type == __NJS_ARRAY) _obj = &_array.get().a->__OBJECT;
    else if(_array.type == __NJS_STRING) _obj = &_array.get().s->__OBJECT;
	else if(_array.type == __NJS_FUNCTION) _obj = &_array.get().f->__OBJECT;
    else
	{
		__NJS_RETURN_UNDEFINED;
	}
    int _j = (*_obj).size();
    for(int _i = 0; _i < _j; _i++)
    {
      if(strcmp(_index, (*_obj)[_i].first) == 0)
      {
        return (*_obj)[_i].second;
      }
    }
  }
  __NJS_RETURN_UNDEFINED;
}

inline __NJS_VAR& __NJS_Object_Get(__NJS_VAR& _index, __NJS_VAR _array)
{
  if(_array.type != __NJS_ARRAY && _array.type != __NJS_OBJECT && _array.type != __NJS_STRING && _array.type != __NJS_FUNCTION) 
  {
	  __NJS_RETURN_UNDEFINED;
  }
  if(_array.type == __NJS_ARRAY && _index.type == __NJS_NUMBER)
  {
    if(_index.get().i > _array.get().a->__NJS_VALUE.size())
    {
      __NJS_RETURN_UNDEFINED;
    }
    return _array.get().a->__NJS_VALUE[_index.get().i];
  }
  else
  {
    if(_index.type != __NJS_STRING)
	{
		__NJS_RETURN_UNDEFINED;
	}
    vector<pair<char*, __NJS_VAR>>* _obj;
    if(_array.type == __NJS_OBJECT) _obj = &_array.get().o->__OBJECT;
	else if(_array.type == __NJS_ARRAY) _obj = &_array.get().a->__OBJECT;
    else if(_array.type == __NJS_STRING) _obj = &_array.get().s->__OBJECT;
	else if(_array.type == __NJS_FUNCTION) _obj = &_array.get().f->__OBJECT;
    else
	{
		__NJS_RETURN_UNDEFINED;
	}
    int _j = (*_obj).size();
    for(int _i = 0; _i < _j; _i++)
    {
      if(strcmp(_index.get().s->__NJS_VALUE.c_str(), (*_obj)[_i].first) == 0)
      {
        return (*_obj)[_i].second;
      }
    }
  }
  __NJS_RETURN_UNDEFINED;
}


__NJS_Class_String::__NJS_Class_String(char* _value)
{
	//cnt++;
	/*** toString ***/
  function<__NJS_VAR ()>* __OBJ_TO___NJS_STRING = new function<__NJS_VAR ()>([&](){ return __NJS_Create_String((char*)this->__NJS_VALUE.c_str()); });
  __NJS_VAR toString = __NJS_VAR(__NJS_FUNCTION, __OBJ_TO___NJS_STRING);
  __NJS_Object_Set((char*)"toString", toString, &this->__OBJECT);
	/*** end to string ***/
	
	/*** length ***/
  __NJS_Object_Set((char*)"length", __NJS_VAR((int)strlen(_value)), &this->__OBJECT);
	/*** end length ***/
	
  /*** split ***/
  function<__NJS_VAR (__NJS_VAR)>* __OBJ_TO___NJS_SPLIT = new function<__NJS_VAR (__NJS_VAR)>([&](__NJS_VAR _needle)
  { 
	  
	__NJS_VAR _arr = __NJS_Create_Array();
    char* _v = (char*)malloc(strlen(this->__NJS_VALUE.c_str()) + 1);
	strcpy(_v, this->__NJS_VALUE.c_str());	
	
	char* delim = (char*)malloc(strlen(_needle.get().s->__NJS_VALUE.c_str()) + 1);
	strcpy(delim, _needle.get().s->__NJS_VALUE.c_str());

	char *ptr = strtok(_v, delim);
	int i = 0;
	char* _new;

	while (ptr != NULL)
	{
		char* _new = (char*)malloc(strlen(ptr));
		strcpy(_new, ptr);
		__NJS_Object_Set(i, _new, _arr);
		ptr = strtok(NULL, delim);
		i++;
	}
	return _arr; 
  });
  
  __NJS_VAR __split = __NJS_VAR(__NJS_FUNCTION, __OBJ_TO___NJS_SPLIT);
  __NJS_Object_Set((char*)"split", __split, &this->__OBJECT);
	/*** end split ***/


  __NJS_VALUE = _value;
}

inline __NJS_VAR __NJS_Class_String::Get(char* _index)
{
  return __NJS_Object_Get(_index, this);
}

__NJS_Class_Array::__NJS_Class_Array()
{
	cnt++;
  function<__NJS_VAR ()>* __OBJ_TO___NJS_STRING = new function<__NJS_VAR ()>([&](){ return __NJS_Create_String((char*)"Array"); });
  __NJS_VAR toString = __NJS_VAR(__NJS_FUNCTION, __OBJ_TO___NJS_STRING);
  __NJS_Object_Set((char*)"toString", toString, &this->__OBJECT);
  
  __NJS_Object_Set((char*)"length", __NJS_VAR(0), &this->__OBJECT);
}

__NJS_Class_Object::__NJS_Class_Object()
{
  cnt++;
}


__NJS_Class_Function::__NJS_Class_Function(void* _f)
{
  cnt++;
  __NJS_VALUE = _f;

}


void __NJS_Class_Object::Delete()
{
	this->cnt --;
	if(this->cnt < 1)
	{
		delete this;
	}	
}

inline __NJS_VAR __NJS_Class_Array::Get(char* _index)
{
  return __NJS_Object_Get(_index, this);
}


inline __NJS_VAR Array()
{
  vector<__NJS_VAR>* _arr = new vector<__NJS_VAR>();
  return __NJS_VAR(__NJS_ARRAY, _arr);
}

inline __NJS_VAR __NJS_Boolean_Result(__NJS_VAR _v)
{
  if(_v.type == __NJS_NUMBER) return _v.get().i;
  else if(_v.type == __NJS_BOOLEAN) return _v.get().b;
  else if(_v.type == __NJS_STRING)
  {
    if(strlen(_v.get().s->__NJS_VALUE.c_str()) > 0) return true;
    else return false;
  }
  else if(_v.type == __NJS_ARRAY) return true;
  else return false;
}

ostream& operator << (ostream& os, const __NJS_VAR& _v)
{
  switch(_v.type)
	{
    case __NJS_UNDEFINED:
		os << "undefined";
		break;
    case __NJS_BOOLEAN:
		if(_v.get().b) os << (char*)"true";
		else os << (char*)"false";
		break;
	case __NJS_NUMBER:
		os << _v.get().i;
		break;
	case __NJS_DOUBLE:
		os << _v.get().d;
		break;
	case __NJS_STRING:
		os << _v.get().s->__NJS_VALUE;
		break;
	default:
		os << "undefined";
	}
	return os;
}

inline __NJS_VAR parseInt(__NJS_VAR _str)
{
  if(_str.type == __NJS_STRING)
  {
	#ifdef __NJS_ARDUINO
		return __NJS_VAR();
	#else
		return __NJS_Create_Number(stoi(_str.get().s->__NJS_VALUE));
	#endif
  }
  else return __NJS_Create_Undefined();
}


inline __NJS_VAR  __NJS_Log_Console(__NJS_VAR _var)
{
	#ifdef __NJS_ARDUINO
	
	#else
		cout << _var;
		cout << endl;
	#endif
	
  return __NJS_VAR();
}


inline __NJS_VAR __NJS_Create_String(char* _value)
{
  __NJS_Class_String* _s = new __NJS_Class_String(_value);
  return __NJS_VAR(_s);
}

inline __NJS_VAR __NJS_Create_Object()
{
  __NJS_Class_Object* _obj = new __NJS_Class_Object();
  return __NJS_VAR(_obj);
}


inline __NJS_VAR Object()
{
  return __NJS_Create_Object();
}

inline __NJS_VAR __NJS_CREATE_FUNCTION(void* _fn)
{
  return __NJS_VAR(__NJS_FUNCTION, _fn);
}

inline __NJS_VAR __NJS_Create_Native(void* _native)
{
  return __NJS_VAR(__NJS_UNDEFINED, _native);
}

inline void* __NJS_Get_Function(__NJS_VAR _fn)
{
  return _fn.get().f;
}

inline void* __NJS_Get_Native(__NJS_VAR _native)
{
  return _native.get().f;
}

/*** REDIFINING STD OPERATORS ***/

template<typename t>
inline __NJS_VAR operator+(t _left, const __NJS_VAR& _right)
{
	return __NJS_VAR(_left) + _right;
}

template<typename t>
inline __NJS_VAR operator-(t _left, const __NJS_VAR& _right)
{
	return __NJS_VAR(_left) - _right;
}

template<typename t>
inline __NJS_VAR operator*(t _left, const __NJS_VAR& _right)
{
	return __NJS_VAR(_left) * _right;
}

template<typename t>
inline __NJS_VAR operator/(t _left, const __NJS_VAR& _right)
{
	return __NJS_VAR(_left) / _right;
}

/*** END REDIFINING STD OPERATORS ***/
