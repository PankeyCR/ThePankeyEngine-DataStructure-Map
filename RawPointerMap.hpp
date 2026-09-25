#pragma once

#if defined(pankey_Log) && (defined(RawPointerMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define RawPointerMapLog(status,method,mns) pankey_Log(status,"RawPointerMap",method,mns)
#else
	#define RawPointerMapLog(status,method,mns)
#endif

/*
	RawPointerMap is an abstract base class for managing a map of raw pointers.
	It provides basic functionalities such as checking if the map is empty,
	getting the length and size, and adding or setting pointers.
	Derived classes must implement the pure virtual functions for specific behaviors.

	Key is never nullptr.
	Value can be nullptr.
*/

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class Policy>
			class RawPointerMap{
				public:
					using Key_Type = typename Policy::Key_Type;
					using Value_Type = typename Policy::Value_Type;

					using Size_Type = typename Policy::Size_Type;

					virtual ~RawPointerMap(){
						RawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						RawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					virtual bool isEmpty()const=0;

					Size_Type length()const{
						RawPointerMapLog(pankey_Log_StartMethod, "length", "");
						RawPointerMapLog(pankey_Log_EndMethod, "length", "");
						return this->m_index;
					}

					Size_Type getSize()const{
						RawPointerMapLog(pankey_Log_StartMethod, "getSize", "");
						RawPointerMapLog(pankey_Log_EndMethod, "getSize", "");
						return this->m_size;
					}
					
					bool hasAvailableSize()const{
						RawPointerMapLog(pankey_Log_StartMethod, "hasAvailableSize", "");
						RawPointerMapLog(pankey_Log_EndMethod, "hasAvailableSize", "");
						return this->m_index < this->m_size  && this->m_size != 0;
					}
					
					bool hasAvailableSize(int a_index)const{
						RawPointerMapLog(pankey_Log_StartMethod, "hasAvailableSize(int)", "");
						RawPointerMapLog(pankey_Log_EndMethod, "hasAvailableSize(int)", "");
						return a_index >= 0 && a_index < this->length() && this->hasAvailableSize();
					}

					void addMove(RawPointerMap<Policy>& a_map){
						RawPointerMapLog(pankey_Log_StartMethod, "addMove", "");
						for(int x = 0; x < a_map.length(); x++){
							Key_Type* k = a_map.getKeyPointerByIndex(x);
							Value_Type* v = a_map.getValuePointerByIndex(x);
							this->addPointers(k,v);
							a_map.releaseKeyPointer(k);
							a_map.releaseValuePointer(v);
						}
						a_map.m_index = 0;
						RawPointerMapLog(pankey_Log_EndMethod, "addMove", "");
					}

					void addDuplicate(const RawPointerMap<Policy>& a_map){
						RawPointerMapLog(pankey_Log_StartMethod, "addDuplicate", "");
						for(int x = 0; x < a_map.length(); x++){
							Key_Type* k = a_map.getKeyPointerByIndex(x);
							Value_Type* v = a_map.getValuePointerByIndex(x);
							this->addPointers(k,v);
							a_map.releaseKeyPointer(k);
							a_map.releaseValuePointer(v);
						}
						RawPointerMapLog(pankey_Log_EndMethod, "addDuplicate", "");
					}

					virtual bool addPointers(Key_Type* a_key, Value_Type* a_value)=0;
					virtual bool addFastPointers(Key_Type* a_key, Value_Type* a_value)=0;

					bool putPointers(Key_Type* a_key, Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "putPointers", "");
						if(this->containPairPointers(a_key,a_value)){
							RawPointerMapLog(pankey_Log_EndMethod, "putPointers", "");
							return true;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "putPointers", "");
						return this->addPointers(a_key, a_value);
					}

					bool setPointers(Key_Type* a_key, Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "setPointers", "");
						int i_index = this->getKeyIndexByPointer(a_key);
						if(i_index == -1){
							RawPointerMapLog(pankey_Log_EndMethod, "setPointers", "");
							return false;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "setPointers", "");
						return this->setValuePointerByIndex(i_index, a_value);
					}

					virtual bool setKeyPointerByIndex(Size_Type a_index, Key_Type* a_key)=0;
					virtual bool setValuePointerByIndex(Size_Type a_index, Value_Type* a_value)=0;

					bool containPairPointers(Key_Type* a_key, Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "containPairPointers", "");
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_key == a_key && i_value == a_value){
								RawPointerMapLog(pankey_Log_EndMethod, "containPairPointers", "");
								return true;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "containPairPointers", "");
						return false;
					}

					bool containKeyByPointer(Key_Type* a_key){
						RawPointerMapLog(pankey_Log_StartMethod, "containKeyByPointer", "");
						for(Size_Type x = 0; x < this->length(); x++){
							Key_Type* i_key = this->getFastKeyPointerByIndex(x);
							if(i_key == a_key){
								RawPointerMapLog(pankey_Log_EndMethod, "containKeyByPointer", "");
								return true;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "containKeyByPointer", "");
						return false;
					}

					bool containValueByPointer(Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "containValueByPointer", "");
						for(Size_Type x = 0; x < this->length(); x++){
							Value_Type* i_value = this->getFastValuePointerByIndex(x);
							if(i_value == a_value){
								RawPointerMapLog(pankey_Log_EndMethod, "containValueByPointer", "");
								return true;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "containValueByPointer", "");
						return false;
					}

					Key_Type* getKeyPointerByPointer(Value_Type* a_value)const{
						RawPointerMapLog(pankey_Log_StartMethod, "getKeyPointerByPointer", "");
						for(Size_Type x = 0; x < this->length(); x++){
							if(this->getFastValuePointerByIndex(x) == a_value){
								RawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByPointer", "");
								return this->getFastKeyPointerByIndex(x);
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByPointer", "");
						return nullptr;
					}

					virtual Key_Type* getKeyPointerByIndex(Size_Type a_index)const=0;
					virtual Key_Type* getFastKeyPointerByIndex(Size_Type a_index)const=0;

					Value_Type* getValuePointerByPointer(const Key_Type* a_key)const{
						RawPointerMapLog(pankey_Log_StartMethod, "getValuePointerByPointer", "");
						for(Size_Type x = 0; x < this->length(); x++){
							if(this->getFastKeyPointerByIndex(x) == a_key){
								RawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByPointer", "");
								return this->getFastValuePointerByIndex(x);
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByPointer", "");
						return nullptr;
					}

					virtual Value_Type* getValuePointerByIndex(Size_Type a_index)const=0;
					virtual Value_Type* getFastValuePointerByIndex(Size_Type a_index)const=0;
					

					virtual void reset()=0;
					virtual void clear()=0;
					virtual void clearValue()=0;

					bool removePointersByKeyPointer(Key_Type* a_key){
						RawPointerMapLog(pankey_Log_StartMethod, "removePointersByKeyPointer", "");
						if(this->isEmpty()){
							RawPointerMapLog(pankey_Log_EndMethod, "removePointersByKeyPointer", "");
							return false;
						}
						Size_Type i_index = 0;
						for(Size_Type x = 0; x < this->length(); x++){
							if(a_key == this->getFastKeyPointerByIndex(x)){
								i_index = x;
								break;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "removePointersByKeyPointer", "");
						return this->removePointersByIndex(i_index);
					}

					bool removePointersByValuePointer(Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "removePointersByValuePointer", "");
						if(this->isEmpty()){
							RawPointerMapLog(pankey_Log_EndMethod, "removePointersByValuePointer", "");
							return false;
						}
						Size_Type i_index = 0;
						for(Size_Type x = 0; x < this->length(); x++){
							if(a_value == this->getFastValuePointerByIndex(x)){
								i_index = x;
								break;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "removePointersByValuePointer", "");
						return this->removePointersByIndex(i_index);
					}

					virtual bool removePointersByIndex(Size_Type a_index)=0;

					bool destroyByKeyPointer(Key_Type* a_key){
						RawPointerMapLog(pankey_Log_StartMethod, "destroyByKeyPointer", "");
						Value_Type* i_value = this->getValuePointerByPointer(a_key);
						if(this->removePointersByKeyPointer(a_key)){
							this->destroyKeyPointer(a_key);
							this->destroyValuePointer(i_value);
							return true;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "destroyByKeyPointer", "");
						return false;
					}

					bool destroyByValuePointer(Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "destroyByValuePointer", "");
						Key_Type* i_key = this->getKeyPointerByPointer(a_value);
						if(this->removePointersByValuePointer(a_value)){
							this->destroyKeyPointer(i_key);
							this->destroyValuePointer(a_value);
							RawPointerMapLog(pankey_Log_EndMethod, "destroyByValuePointer", "");
							return true;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "destroyByValuePointer", "");
						return false;
					}

					bool destroyByIndex(Size_Type a_index){
						RawPointerMapLog(pankey_Log_StartMethod, "destroyByIndex", "");
						Key_Type* i_key = this->getFastKeyPointerByIndex(a_index);
						Value_Type* i_value = this->getFastValuePointerByIndex(a_index);
						if(this->removePointersByIndex(a_index)){
							this->destroyKeyPointer(i_key);
							this->destroyValuePointer(i_value);
							RawPointerMapLog(pankey_Log_EndMethod, "destroyByIndex", "");
							return true;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "destroyByIndex", "");
						return false;
					}

					Size_Type getKeyIndexByPointer(Key_Type* a_key){
						RawPointerMapLog(pankey_Log_StartMethod, "getKeyIndexByPointer", "");
						if(this->isEmpty()){
							RawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "");
							return static_cast<Size_Type>(-1);
						}
						for(Size_Type x = 0; x < this->length(); x++){
							if(a_key == this->getFastKeyPointerByIndex(x)){
								RawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "");
								return x;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "");
						return static_cast<Size_Type>(-1);
					}

					Size_Type getValueIndexByPointer(Value_Type* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "getValueIndexByPointer", "");
						if(this->isEmpty()){
							RawPointerMapLog(pankey_Log_EndMethod, "getValueIndexByPointer", "");
							return static_cast<Size_Type>(-1);
						}
						for(Size_Type x = 0; x < this->length(); x++){
							if(a_value == this->getFastValuePointerByIndex(x)){
								RawPointerMapLog(pankey_Log_EndMethod, "getValueIndexByPointer", "");
								return x;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "getValueIndexByPointer", "");
						return static_cast<Size_Type>(-1);
					}

				protected:

					void incrementIndex(){
						RawPointerMapLog(pankey_Log_StartMethod, "incrementIndex", static_cast<Size_Type>(this->m_index));
						this->m_index++;
						RawPointerMapLog(pankey_Log_EndMethod, "incrementIndex", static_cast<Size_Type>(this->m_index));
					}
					void decrementIndex(){
						RawPointerMapLog(pankey_Log_StartMethod, "decrementIndex", static_cast<Size_Type>(this->m_index));
						this->m_index--;
						if(this->m_index < 0){
							this->m_index = 0;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "decrementIndex", static_cast<Size_Type>(this->m_index));
					}

					void incrementIndex(int a_size){
						RawPointerMapLog(pankey_Log_StartMethod, "incrementIndex", static_cast<Size_Type>(this->m_index));
						this->m_index += a_size;
						RawPointerMapLog(pankey_Log_EndMethod, "incrementIndex", static_cast<Size_Type>(this->m_index));
					}
					void decrementIndex(int a_size){
						RawPointerMapLog(pankey_Log_StartMethod, "decrementIndex", static_cast<Size_Type>(this->m_index));
						this->m_index -= a_size;
						if(this->m_index < 0){
							this->m_index = 0;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "decrementIndex", static_cast<Size_Type>(this->m_index));
					}
					
					bool isEqualKey(Key_Type* a_key_1, Key_Type* a_key_2){
						if(a_key_1 != nullptr && a_key_2 != nullptr){
							if(*a_key_1 != *a_key_2){
								return false;
							}
						}else{
							if((a_key_1 != nullptr || a_key_2 != nullptr) && (a_key_1 != a_key_2)){
								return false;
							}
						}
						return true;
					}
					
					bool isEqualValue(Value_Type* a_value_1, Value_Type* a_value_2){
						if(a_value_1 != nullptr && a_value_2 != nullptr){
							if(*a_value_1 != *a_value_2){
								return false;
							}
						}else{
							if((a_value_1 != nullptr || a_value_2 != nullptr) && (a_value_1 != a_value_2)){
								return false;
							}
						}
						return true;
					}
					
				protected:
					virtual void attachKeyPointer(Key_Type* a_pointer){}
					virtual void attachValuePointer(Value_Type* a_pointer){}
					
					virtual void releaseKeyPointer(Key_Type* a_pointer){}
					virtual void releaseValuePointer(Value_Type* a_pointer){}
					
					virtual void destroyKeyPointer(Key_Type* a_pointer){}
					virtual void destroyValuePointer(Value_Type* a_pointer){}
					
					Size_Type m_index = 0;
					Size_Type m_size = 0;
			};

		}

	}

}
