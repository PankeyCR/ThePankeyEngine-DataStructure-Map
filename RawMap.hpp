#pragma once

#include "RawPointerMap.hpp"

#if defined(pankey_Log) && (defined(RawMap_Log) || defined(pankey_Global_Log) || defined(pankey_DataStructure_Map_Log))
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

						this->expandAutomatic();

						if(!this->hasAvailableSize()){
							RawMapLog(pankey_Log_Statement, "addPointers", "!this->hasAvailableSize()");
							return false;
						}

						Key_Type* i_key_pointer = this->createKeyPointer();
						if(i_key_pointer != nullptr){
							*i_key_pointer = a_key;
						}

						RawMapLog(pankey_Log_EndMethod, "addPointer", "");
						return this->addFastPointers(i_key_pointer, a_value);
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

					bool setPointer(Key_Type a_key, Value_Type* a_value){
						RawMapLog(pankey_Log_StartMethod, "setPointer", "");
						Size_Type i_index = this->getKeyIndex(a_key);
						if(i_index == Policy::UNDEFINED_SIZE){
							RawMapLog(pankey_Log_EndMethod, "setPointer", "");
							return false;
						}
						RawMapLog(pankey_Log_EndMethod, "setPointer", "");
						return this->setValuePointerByIndex(i_index, a_value);
					}

					bool setKeyByIndex(int a_index, Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "setKeyByIndex", "");
						if(!this->hasAvailableSize(a_index)){
							RawMapLog(pankey_Log_EndMethod, "setKeyByIndex", "");
							return false;
						}
						Key_Type* i_key = this->getKeyPointerByIndex(a_index);

						if(i_key == nullptr){
							return false;
						}

						if(a_key == *i_key){
							RawMapLog(pankey_Log_EndMethod, "setKeyByIndex", "");
							return true;
						}
						
						*i_key = a_key;
						
						RawMapLog(pankey_Log_EndMethod, "setKeyByIndex", "");
						return true;
					}
					
					bool containKey(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "containKey", "");

						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(*i_key == a_key){
								return true;
							}
						}

						RawMapLog(pankey_Log_EndMethod, "containKey", "");
						return false;
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
						if(i_index == Policy::UNDEFINED_SIZE){
							RawMapLog(pankey_Log_EndMethod, "removeByKey", "");
							return false;
						}
						RawMapLog(pankey_Log_EndMethod, "removeByKey", "");
						return this->removePointersByIndex(i_index);
					}
					
					bool destroyByKey(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "destroyByKey", "");
						Value_Type* i_value = this->getValuePointer(a_key);
						Key_Type* i_key = this->getKeyPointerByPointer(i_value);
						if(this->removePointersByValuePointer(i_value)){
							this->destroyKeyPointer(i_key);
							this->destroyValuePointer(i_value);
							return true;
						}
						RawMapLog(pankey_Log_EndMethod, "destroyByKey", "");
						return false;
					}
					
					int getKeyIndex(Key_Type a_key){
						RawMapLog(pankey_Log_StartMethod, "getKeyIndex", "");
						if(this->isEmpty()){
							RawMapLog(pankey_Log_EndMethod, "getKeyIndex", "");
							return Policy::UNDEFINED_SIZE;
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
						return Policy::UNDEFINED_SIZE;
					}
			};

		}

	}

}