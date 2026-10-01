#pragma once

#include "RawPointerMap.hpp"

#if defined(pankey_Log) && (defined(RawMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define RawMapLog(status,method,mns) pankey_Log(status,"RawMap",method,mns)
#else
	#define RawMapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class Policy>
			class RawMap : virtual public RawPointerMap<Policy>{
				public:
					using Key_Type = typename Policy::Key_Type;
					using Value_Type = typename Policy::Value_Type;

					using Size_Type = typename Policy::Size_Type;

					virtual ~RawMap(){
						RawMapLog(pankey_Log_StartMethod, "Destructor", "");
						RawMapLog(pankey_Log_EndMethod, "Destructor", "");
					}
					
					bool addPointer(Key_Type a_key, Value_Type* a_value){
						RawMapLog(pankey_Log_StartMethod, "addPointer", "");
						if(!this->hasAvailableSize()){
							RawMapLog(pankey_Log_Statement, "addPointers", "!this->hasAvailableSize()");
							return false;
						}

						Key_Type* i_key_pointer = this->createKeyPointer();
						*i_key_pointer = a_key;

						RawMapLog(pankey_Log_EndMethod, "addPointer", "");
						return this->addFastPointers(i_key_pointer, a_value);
					}
					
					bool addValues(Key_Type a_key, Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "addPointer", "");
						if(!this->hasAvailableSize()){
							RawMapLog(pankey_Log_Statement, "addPointers", "!this->hasAvailableSize()");
							return false;
						}

						Key_Type* i_key_pointer = this->createKeyPointer();
						*i_key_pointer = a_key;

						Value_Type* i_value_pointer = this->createValuePointer();
						*i_value_pointer = a_value;

						RawMapLog(pankey_Log_EndMethod, "addPointer", "");
						return this->addFastPointers(i_key_pointer, i_value_pointer);
					}
					
					bool putPointer(Key_Type a_key, Value_Type* a_value){
						RawMapLog(pankey_Log_StartMethod, "putPointer", "");
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								RawMapLog(pankey_Log_EndMethod, "putPointer", "");
								return this->setValuePointerByIndex(x, a_value);
							}
						}
						RawMapLog(pankey_Log_EndMethod, "putPointer", "");
						return this->addPointer(a_key, a_value);
					}
					
					bool putValues(Key_Type a_key, Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "putValues", "");
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								Value_Type* i_value_pointer = this->createValuePointer();
								*i_value_pointer = a_value;
								RawMapLog(pankey_Log_EndMethod, "putValues", "");
								return this->setValuePointerByIndex(x, i_value_pointer);
							}
						}
						RawMapLog(pankey_Log_EndMethod, "putValues", "");
						return this->addValues(a_key, a_value);
					}

					bool setPointer(Key_Type a_key, Value_Type* a_value){
						RawMapLog(pankey_Log_StartMethod, "setPointers", "");
						Size_Type i_index = this->getKeyIndex(a_key);
						if(i_index == static_cast<Size_Type>(-1)){
							RawMapLog(pankey_Log_EndMethod, "setPointers", "");
							return false;
						}
						RawMapLog(pankey_Log_EndMethod, "setPointers", "");
						return this->setValuePointerByIndex(i_index, a_value);
					}

					bool setValues(Key_Type a_key, Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "setPointers", "");
						Size_Type i_index = this->getKeyIndex(a_key);
						if(i_index == static_cast<Size_Type>(-1)){
							RawMapLog(pankey_Log_EndMethod, "setPointers", "");
							return false;
						}
						RawMapLog(pankey_Log_EndMethod, "setPointers", "");
						return this->setValueByIndex(i_index, a_value);
					}

					bool setKeyByIndex(int a_index, Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "setKeyPointerByIndex", "");
						if(!this->hasAvailableSize(a_index)){
							RawMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return false;
						}
						Key_Type* i_key = this->getKeyPointerByIndex(a_index);

						if(i_key == nullptr){
							return false;
						}

						if(a_key == *i_key){
							RawMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return true;
						}
						
						*i_key = a_key;
						
						RawMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
						return true;
					}

					bool setValueByIndex(int a_index, Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "setValuePointerByIndex", "");
						if(!this->hasAvailableSize(a_index)){
							RawMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return false;
						}
						Value_Type* i_value = this->getValuePointerByIndex(a_index);

						if(i_value == nullptr){
							Value_Type* i_value_pointer = this->createValuePointer();
							*i_value_pointer = a_value;
							return this->setValuePointerByIndex(a_index, i_value_pointer);
						}

						if(a_value == *i_value){
							RawMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return true;
						}
						
						*i_value = a_value;
						
						RawMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
						return true;
					}
					
					bool containPairValues(Key_Type a_key, Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "getValuePointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_key == nullptr || i_value == nullptr){
								continue;
							}
							if(*i_key == a_key && *i_value == a_value){
								return true;
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getValuePointer", "");
						return false;
					}
					
					bool containKey(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "getValuePointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								return true;
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getValuePointer", "");
						return false;
					}

					bool containValue(Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "getKeyPointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(*i_value == a_value){
								return true;
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getKeyPointer", "");
						return false;
					}
					
					Key_Type* getKeyPointer(Value_Type a_value)const{
						RawMapLog(pankey_Log_StartMethod, "getKeyPointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(*i_value == a_value){
								return this->getFastKeyPointerByIndex(x);
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getKeyPointer", "");
						return nullptr;
					}
					
					Key_Type getKey(Value_Type a_value)const{
						RawMapLog(pankey_Log_StartMethod, "getKeyPointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(*i_value == a_value){
								Key_Type* i_key = this->getFastKeyPointerByIndex(x);
								if(i_key == nullptr){
									return Key_Type();
								}
								return *i_key;
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getKeyPointer", "");
						return Key_Type();
					}
					
					Value_Type* getValuePointer(Key_Type a_key)const{
						RawMapLog(pankey_Log_StartMethod, "getValuePointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								return this->getFastValuePointerByIndex(x);
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getValuePointer", "");
						return nullptr;
					}
					
					Value_Type getValue(Key_Type a_key)const{
						RawMapLog(pankey_Log_StartMethod, "getValuePointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								Value_Type* i_value = this->getFastValuePointerByIndex(x);
								if(i_value == nullptr){
									return Value_Type();
								}
								return *i_value;
							}
						}

						RawMapLog(pankey_Log_EndMethod, "getValuePointer", "");
						return Value_Type();
					}
					
					Key_Type getKeyByIndex(int a_index){
						RawMapLog(pankey_Log_StartMethod, "getKeyByIndex", "");
						Key_Type* i_key = this->getKeyPointerByIndex(a_index);
						if(i_key == nullptr){
							RawMapLog(pankey_Log_EndMethod, "getKeyByIndex", "");
							return Key_Type();
						}
						RawMapLog(pankey_Log_EndMethod, "getKeyByIndex", "");
						return *i_key;
					}

					Value_Type getValueByIndex(int a_index){
						RawMapLog(pankey_Log_StartMethod, "getValueByIndex", "");
						Value_Type* i_value = this->getValuePointerByIndex(a_index);
						if(i_value == nullptr){
							RawMapLog(pankey_Log_EndMethod, "getValueByIndex", "");
							return Value_Type();
						}
						RawMapLog(pankey_Log_EndMethod, "getValueByIndex", "");
						return *i_value;
					}
					
					bool removeByKey(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "removeByKey", "");
						if(this->isEmpty()){
							RawMapLog(pankey_Log_EndMethod, "removeByKey", "");
							return false;
						}
						Size_Type i_index = -1;
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* f_key_pointer = this->getFastKeyPointerByIndex(x);
							if(a_key == *f_key_pointer){
								i_index = x;
								break;
							}
						}
						if(i_index == static_cast<Size_Type>(-1)){
							RawMapLog(pankey_Log_EndMethod, "removeByKey", "");
							return false;
						}
						RawMapLog(pankey_Log_EndMethod, "removeByKey", "");
						return this->removePointersByIndex(i_index);
					}

					bool removeByValue(Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "removeByValue", "");
						if(this->isEmpty()){
							RawMapLog(pankey_Log_EndMethod, "removeByValue", "");
							return false;
						}
						Size_Type i_index = -1;
						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value_pointer = this->getFastValuePointerByIndex(x);
							if(a_value == *i_value_pointer){
								i_index = x;
								break;
							}
						}
						if(i_index == static_cast<Size_Type>(-1)){
							RawMapLog(pankey_Log_EndMethod, "removeByValue", "");
							return false;
						}
						RawMapLog(pankey_Log_EndMethod, "removeByValue", "");
						return this->removePointersByIndex(i_index);
					}
					
					bool destroyByKey(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "destroyByKeyPointer", "");
						Value_Type* i_value = this->getValuePointer(a_key);
						Key_Type* i_key = this->getKeyPointerByPointer(i_value);
						if(this->removePointersByValuePointer(i_value)){
							this->destroyKeyPointer(i_key);
							this->destroyValuePointer(i_value);
							return true;
						}
						RawMapLog(pankey_Log_EndMethod, "destroyByKeyPointer", "");
						return false;
					}

					virtual bool destroyByValue(Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "destroyByValue", "");
						Key_Type* i_key = this->getKeyPointer(a_value);
						Value_Type* i_value = this->getValuePointerByPointer(i_key);
						if(this->removePointersByKeyPointer(i_key)){
							this->destroyKeyPointer(i_key);
							this->destroyValuePointer(i_value);
							RawMapLog(pankey_Log_EndMethod, "destroyByValue", "");
							return true;
						}
						RawMapLog(pankey_Log_EndMethod, "destroyByValue", "");
						return false;
					}
					
					int getKeyIndex(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "getKeyIndex", "");
						if(this->isEmpty()){
							RawMapLog(pankey_Log_EndMethod, "getKeyIndex", "");
							return static_cast<Size_Type>(-1);
						}
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							
							if(a_key == *i_key){
								RawMapLog(pankey_Log_EndMethod, "getKeyIndex", "");
								return x;
							}
						}
						RawMapLog(pankey_Log_EndMethod, "getKeyIndex", "");
						return static_cast<Size_Type>(-1);
					}
					
					int getValueIndex(Value_Type a_value){
						RawMapLog(pankey_Log_StartMethod, "getValueIndex", "");
						if(this->isEmpty()){
							RawMapLog(pankey_Log_EndMethod, "getValueIndex", "");
							return static_cast<Size_Type>(-1);
						}
						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(a_value == *i_value){
								RawMapLog(pankey_Log_EndMethod, "getValueIndex", "");
								return x;
							}
						}
						RawMapLog(pankey_Log_EndMethod, "getValueIndex", "");
						return static_cast<Size_Type>(-1);
					}
					
					template<class... Args>
					void addKeyPack(Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addKeyPack", "");
						Key_Type i_array[] = {a_values...};
						for(const Key_Type& k : i_array){
							this->addPointers(new Key_Type(k), new Value_Type());
						}
						RawMapLog(pankey_Log_EndMethod, "addKeyPack", "");
					}
					
					template<class... Args>
					void addValuePack(Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addValuePack", "");
						Value_Type i_array[] = {a_values...};
						for(const Value_Type& v : i_array){
							this->addPointers(new Key_Type(), new Value_Type(v));
						}
						RawMapLog(pankey_Log_EndMethod, "addValuePack", "");
					}
					
					template<class... Args>
					void addKeyPack(Value_Type v, Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addKeyPack", "");
						Key_Type i_array[] = {a_values...};
						for(const Key_Type& k : i_array){
							this->addPointers(new Key_Type(k), new Value_Type(v));
						}
						RawMapLog(pankey_Log_EndMethod, "addKeyPack", "");
					}
					
					template<class... Args>
					void addValuePack(Key_Type k, Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addValuePack", "");
						Value_Type i_array[] = {a_values...};
						for(const Value_Type& v : i_array){
							this->addPointers(new Key_Type(k), new Value_Type(v));
						}
						RawMapLog(pankey_Log_EndMethod, "addValuePack", "");
					}
			};

		}

	}

}