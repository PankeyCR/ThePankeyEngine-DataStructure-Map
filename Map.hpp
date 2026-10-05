#pragma once

#include "RawMap.hpp"

#if defined(pankey_Log) && (defined(Map_Log) || defined(pankey_Global_Log) || defined(pankey_DataStructure_Map_Log))
	#include "Logger_status.hpp"
	#define MapLog(status,method,mns) pankey_Log(status,"Map",method,mns)
#else
	#define MapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class Policy>
			class Map : public RawMap<Policy>{
				public:
					using Key_Type = typename Policy::Key_Type;
					using Value_Type = typename Policy::Value_Type;

					using Size_Type = typename Policy::Size_Type;
					
					virtual ~Map(){
						MapLog(pankey_Log_StartMethod, "Destructor", "");
						MapLog(pankey_Log_EndMethod, "Destructor", "");
					}
					
					bool add(Key_Type a_key, Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "add", "");

						this->expandAutomatic();

						if(!this->hasAvailableSize()){
							MapLog(pankey_Log_Statement, "add", "!this->hasAvailableSize()");
							return false;
						}

						Key_Type* i_key_pointer = this->createKeyPointer();
						if(i_key_pointer != nullptr){
							*i_key_pointer = a_key;
						}

						Value_Type* i_value_pointer = this->createValuePointer();
						if(i_value_pointer != nullptr){
							*i_value_pointer = a_value;
						}

						MapLog(pankey_Log_EndMethod, "add", "");
						return this->addFastPointers(i_key_pointer, i_value_pointer);
					}
					
					bool put(Key_Type a_key, Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "put", "");
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								Value_Type* i_value_pointer = this->createValuePointer();
								if(i_value_pointer != nullptr){
									*i_value_pointer = a_value;
								}
								MapLog(pankey_Log_EndMethod, "put", "");
								return this->setValuePointerByIndex(x, i_value_pointer);
							}
						}
						MapLog(pankey_Log_EndMethod, "put", "");
						return this->add(a_key, a_value);
					}

					bool set(Key_Type a_key, Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "set", "");
						Size_Type i_index = this->getKeyIndex(a_key);
						if(i_index == Policy::UNDEFINED_SIZE){
							MapLog(pankey_Log_EndMethod, "set", "");
							return false;
						}
						MapLog(pankey_Log_EndMethod, "set", "");
						return this->setValueByIndex(i_index, a_value);
					}

					bool setValueByIndex(int a_index, Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "setValueByIndex", "");
						if(!this->hasAvailableSize(a_index)){
							MapLog(pankey_Log_EndMethod, "setValueByIndex", "");
							return false;
						}
						Value_Type* i_value = this->getValuePointerByIndex(a_index);

						if(i_value == nullptr){
							Value_Type* i_value_pointer = this->createValuePointer();
							if(i_value_pointer != nullptr){
								*i_value_pointer = a_value;
							}
							return this->setValuePointerByIndex(a_index, i_value_pointer);
						}

						if(a_value == *i_value){
							MapLog(pankey_Log_EndMethod, "setValueByIndex", "");
							return true;
						}
						
						*i_value = a_value;
						
						MapLog(pankey_Log_EndMethod, "setValueByIndex", "");
						return true;
					}
					
					bool containPairValues(Key_Type a_key, Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "containPairValues", "");

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

						MapLog(pankey_Log_EndMethod, "containPairValues", "");
						return false;
					}

					bool containValue(Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "containValue", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(*i_value == a_value){
								return true;
							}
						}

						MapLog(pankey_Log_EndMethod, "containValue", "");
						return false;
					}
					
					Key_Type getKey(Value_Type a_value)const{
						MapLog(pankey_Log_StartMethod, "getKey", "");

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

						MapLog(pankey_Log_EndMethod, "getKey", "");
						return Key_Type();
					}
					
					Key_Type* getKeyPointer(Value_Type a_value)const{
						MapLog(pankey_Log_StartMethod, "getKeyPointer", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(*i_value == a_value){
								return this->getFastKeyPointerByIndex(x);
							}
						}

						MapLog(pankey_Log_EndMethod, "getKeyPointer", "");
						return nullptr;
					}
					
					Value_Type getValue(Key_Type a_key)const{
						MapLog(pankey_Log_StartMethod, "getValue", "");

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

						MapLog(pankey_Log_EndMethod, "getValue", "");
						return Value_Type();
					}

					Value_Type getValueByIndex(int a_index){
						MapLog(pankey_Log_StartMethod, "getValueByIndex", "");
						Value_Type* i_value = this->getValuePointerByIndex(a_index);
						if(i_value == nullptr){
							MapLog(pankey_Log_EndMethod, "getValueByIndex", "");
							return Value_Type();
						}
						MapLog(pankey_Log_EndMethod, "getValueByIndex", "");
						return *i_value;
					}

					bool removeByValue(Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "removeByValue", "");
						if(this->isEmpty()){
							MapLog(pankey_Log_EndMethod, "removeByValue", "");
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
						if(i_index == Policy::UNDEFINED_SIZE){
							MapLog(pankey_Log_EndMethod, "removeByValue", "");
							return false;
						}
						MapLog(pankey_Log_EndMethod, "removeByValue", "");
						return this->removePointersByIndex(i_index);
					}

					virtual bool destroyByValue(Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "destroyByValue", "");
						Key_Type* i_key = this->getKeyPointer(a_value);
						Value_Type* i_value = this->getValuePointerByPointer(i_key);
						if(this->removePointersByKeyPointer(i_key)){
							this->destroyKeyPointer(i_key);
							this->destroyValuePointer(i_value);
							MapLog(pankey_Log_EndMethod, "destroyByValue", "");
							return true;
						}
						MapLog(pankey_Log_EndMethod, "destroyByValue", "");
						return false;
					}
					
					int getValueIndex(Value_Type a_value){
						MapLog(pankey_Log_StartMethod, "getValueIndex", "");
						if(this->isEmpty()){
							MapLog(pankey_Log_EndMethod, "getValueIndex", "");
							return Policy::UNDEFINED_SIZE;
						}
						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(a_value == *i_value){
								MapLog(pankey_Log_EndMethod, "getValueIndex", "");
								return x;
							}
						}
						MapLog(pankey_Log_EndMethod, "getValueIndex", "");
						return Policy::UNDEFINED_SIZE;
					}
					
					template<class... Args>
					void addKeyPack(Args... a_values){
						MapLog(pankey_Log_StartMethod, "addKeyPack", "");
						Key_Type i_array[] = {a_values...};
						for(const Key_Type& k : i_array){
							Key_Type* f_key = this->createKeyPointer();
							if(f_key == nullptr){
								continue;
							}
							*f_key = k;
							Value_Type* f_value = this->createValuePointer();
							this->addPointers(f_key, f_value);
						}
						MapLog(pankey_Log_EndMethod, "addKeyPack", "");
					}
					
					template<class... Args>
					void addKeyPack(Value_Type v, Args... a_values){
						MapLog(pankey_Log_StartMethod, "addKeyPack", "");
						Key_Type i_array[] = {a_values...};
						for(const Key_Type& k : i_array){
							Key_Type* f_key = this->createKeyPointer();
							if(f_key == nullptr){
								continue;
							}
							*f_key = k;
							Value_Type* f_value = this->createValuePointer();
							if(f_value != nullptr){
								*f_value = v;
							}
							this->addPointers(f_key, f_value);
						}
						MapLog(pankey_Log_EndMethod, "addKeyPack", "");
					}
					
					template<class... Args>
					void addValuePack(Key_Type k, Args... a_values){
						MapLog(pankey_Log_StartMethod, "addValuePack", "");
						Value_Type i_array[] = {a_values...};
						for(const Value_Type& v : i_array){
							Key_Type* f_key = this->createKeyPointer();
							if(f_key == nullptr){
								continue;
							}
							*f_key = k;
							Value_Type* f_value = this->createValuePointer();
							if(f_value != nullptr){
								*f_value = v;
							}
							this->addPointers(f_key, f_value);
						}
						MapLog(pankey_Log_EndMethod, "addValuePack", "");
					}
			};

		}

	}

}