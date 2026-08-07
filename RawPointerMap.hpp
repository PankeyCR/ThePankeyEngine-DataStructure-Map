#pragma once

#include "RawMapEntry.hpp"

#if defined(pankey_Log) && (defined(RawPointerMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define RawPointerMapLog(status,method,mns) pankey_Log(status,"RawPointerMap",method,mns)
#else
	#define RawPointerMapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class K,class V>
			class RawPointerMap{
				public:
					virtual ~RawPointerMap(){
						RawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						RawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					virtual bool isEmpty()const=0;

					virtual void setOwner(bool a_owning){
						RawPointerMapLog(pankey_Log_StartMethod, "setOwner", "");
						this->m_key_owner = a_owning;
						this->m_value_owner = a_owning;
						RawPointerMapLog(pankey_Log_EndMethod, "setOwner", "");
					}
					virtual bool isOwner()const{
						RawPointerMapLog(pankey_Log_StartMethod, "isOwner", "");
						RawPointerMapLog(pankey_Log_EndMethod, "isOwner", "");
						return this->m_key_owner && this->m_value_owner;
					}
					virtual bool hasOwner()const{
						RawPointerMapLog(pankey_Log_StartMethod, "isOwner", "");
						RawPointerMapLog(pankey_Log_EndMethod, "isOwner", "");
						return this->m_key_owner || this->m_value_owner;
					}

					virtual void setKeyOwner(bool a_owning){
						RawPointerMapLog(pankey_Log_StartMethod, "setKeyOwner", "");
						this->m_key_owner = a_owning;
						RawPointerMapLog(pankey_Log_EndMethod, "setKeyOwner", "");
					}
					virtual bool isKeyOwner()const{
						RawPointerMapLog(pankey_Log_StartMethod, "isKeyOwner", "");
						RawPointerMapLog(pankey_Log_EndMethod, "isKeyOwner", "");
						return this->m_key_owner;
					}

					virtual void setValueOwner(bool a_owning){
						RawPointerMapLog(pankey_Log_StartMethod, "setValueOwner", "");
						this->m_value_owner = a_owning;
						RawPointerMapLog(pankey_Log_EndMethod, "setValueOwner", "");
					}

					virtual bool isValueOwner()const{
						RawPointerMapLog(pankey_Log_StartMethod, "isValueOwner", "");
						RawPointerMapLog(pankey_Log_EndMethod, "isValueOwner", "");
						return this->m_value_owner;
					}

					virtual void length(int a_index){
						RawPointerMapLog(pankey_Log_StartMethod, "length", a_index);
						this->m_index = a_index;
						RawPointerMapLog(pankey_Log_EndMethod, "length", "");
					}
					
					virtual int length()const{
						RawPointerMapLog(pankey_Log_StartMethod, "length", "");
						RawPointerMapLog(pankey_Log_EndMethod, "length", "");
						return this->m_index;
					}

					virtual void setSize(int a_size){
						RawPointerMapLog(pankey_Log_StartMethod, "setSize", "");
						this->m_size = a_size;
						RawPointerMapLog(pankey_Log_EndMethod, "setSize", "");
					}
					virtual int getSize()const{
						RawPointerMapLog(pankey_Log_StartMethod, "getSize", "");
						RawPointerMapLog(pankey_Log_EndMethod, "getSize", "");
						return this->m_size;
					}
					
					virtual int hasAvailableSize()const{
						RawPointerMapLog(pankey_Log_StartMethod, "getSize", "");
						RawPointerMapLog(pankey_Log_EndMethod, "getSize", "");
						return this->m_index < this->m_size  && this->m_size != 0;
					}

					virtual void addMove(RawPointerMap<K,V>& a_map){
						RawPointerMapLog(pankey_Log_StartMethod, "addMove", "");
						for(int x = 0; x < a_map.length(); x++){
							K* k = a_map.getKeyPointerByIndex(x);
							V* v = a_map.getValuePointerByIndex(x);
							this->addPointers(k,v);
						}
						a_map.reset();
						RawPointerMapLog(pankey_Log_EndMethod, "addMove", "");
					}

					virtual void addDuplicate(const RawPointerMap<K,V>& a_map){
						RawPointerMapLog(pankey_Log_StartMethod, "addDuplicate", "");
						for(int x = 0; x < a_map.length(); x++){
							K* k = a_map.getKeyPointerByIndex(x);
							V* v = a_map.getValuePointerByIndex(x);
							this->addPointers(k,v);
						}
						RawPointerMapLog(pankey_Log_EndMethod, "addDuplicate", "");
					}

					virtual RawMapEntry<K,V> addPointers(K* a_key, V* a_value)=0;
					virtual RawMapEntry<K,V> addRawMapEntry(RawMapEntry<K,V> a_map_entry)=0;

					virtual RawMapEntry<K,V> putPointers(K* a_key, V* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "putPointers", "");
						if(this->containPairPointers(a_key,a_value)){
							RawPointerMapLog(pankey_Log_EndMethod, "putPointers", "");
							return RawMapEntry<K,V>(a_key, a_value);
						}
						RawPointerMapLog(pankey_Log_EndMethod, "putPointers", "");
						return this->addPointers(a_key, a_value);
					}
					virtual RawMapEntry<K,V> putRawMapEntry(RawMapEntry<K,V> a_map_entry){
						RawPointerMapLog(pankey_Log_StartMethod, "putRawMapEntry", "");
						if(this->containRawMapEntry(a_map_entry)){
							RawPointerMapLog(pankey_Log_EndMethod, "putRawMapEntry", "");
							return RawMapEntry<K,V>(a_map_entry);
						}
						RawPointerMapLog(pankey_Log_EndMethod, "putRawMapEntry", "");
						return this->addRawMapEntry(a_map_entry);
					}

					virtual RawMapEntry<K,V> setPointers(K* a_key, V* a_value)=0;
					virtual RawMapEntry<K,V> setRawMapEntry(RawMapEntry<K,V> a_map_entry)=0;

					virtual RawMapEntry<K,V> setKeyPointerByIndex(int a_index, K* a_key)=0;
					virtual RawMapEntry<K,V> setValuePointerByIndex(int a_index, V* a_value)=0;

					virtual RawMapEntry<K,V> setRawMapEntryByIndex(int a_index, RawMapEntry<K,V> a_map_entry)=0;

					virtual bool containPairPointers(K* a_key, V* a_value)=0;
					virtual bool containRawMapEntry(RawMapEntry<K,V> a_map_entry)=0;

					virtual bool containKeyByPointer(K* a_key)=0;
					virtual bool containValueByPointer(V* a_value)=0;

					virtual RawMapEntry<K,V> getRawMapEntryByIndex(int a_index)const=0;

					virtual K* getKeyPointerByPointer(V* a_value)=0;
					virtual K* getKeyPointerByIndex(int a_index)const=0;

					virtual V* getValuePointerByPointer(const K* a_key)const=0;
					virtual V* getValuePointerByIndex(int a_index)const=0;

					virtual void reset()=0;
					virtual void clear()=0;
					virtual void clearKey()=0;
					virtual void clearValue()=0;

					virtual RawMapEntry<K,V> removePointersByKeyPointer(K* a_key)=0;
					virtual RawMapEntry<K,V> removePointersByValuePointer(V* a_value)=0;
					virtual RawMapEntry<K,V> removePointersByIndex(int a_index)=0;

					virtual bool destroyByKeyPointer(K* a_key){
						RawPointerMapLog(pankey_Log_StartMethod, "destroyByKeyPointer", "");
						RawMapEntry<K,V> i_entry = this->removePointersByKeyPointer(a_key);
						bool removed = !i_entry.isNull();
						if(removed && this->isOwner()){
							this->destroyKeyPointer(i_entry.getKey());
							this->destroyValuePointer(i_entry.getValue());
						}
						RawPointerMapLog(pankey_Log_EndMethod, "destroyByKeyPointer", "");
						return removed;
					}

					virtual bool destroyByValuePointer(V* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "destroyByValuePointer", "");
						RawMapEntry<K,V> i_entry = this->removePointersByValuePointer(a_value);
						bool removed = !i_entry.isNull();
						if(removed && this->isOwner()){
							this->destroyKeyPointer(i_entry.getKey());
							this->destroyValuePointer(i_entry.getValue());
						}
						RawPointerMapLog(pankey_Log_EndMethod, "destroyByValuePointer", "");
						return removed;
					}

					virtual bool destroyByIndex(int a_index){
						RawPointerMapLog(pankey_Log_StartMethod, "destroyByIndex", "");
						RawMapEntry<K,V> i_entry = this->removePointersByIndex(a_index);
						bool removed = !i_entry.isNull();
						if(removed && isOwner()){
							this->destroyKeyPointer(i_entry.getKey());
							this->destroyValuePointer(i_entry.getValue());
						}
						RawPointerMapLog(pankey_Log_EndMethod, "destroyByIndex", "");
						return removed;
					}

					virtual int getKeyIndexByPointer(K* a_key){
						RawPointerMapLog(pankey_Log_StartMethod, "getKeyIndexByPointer", "");
						if(this->isEmpty()){
							RawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "");
							return -1;
						}
						for(int x = 0; x < this->length(); x++){
							if(a_key == this->getKeyPointerByIndex(x)){
								RawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "");
								return x;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "");
						return -1;
					}

					virtual int getValueIndexByPointer(V* a_value){
						RawPointerMapLog(pankey_Log_StartMethod, "getValueIndexByPointer", "");
						if(this->isEmpty()){
							RawPointerMapLog(pankey_Log_EndMethod, "getValueIndexByPointer", "");
							return -1;
						}
						for(int x = 0; x < this->length(); x++){
							if(a_value == this->getValuePointerByIndex(x)){
								RawPointerMapLog(pankey_Log_EndMethod, "getValueIndexByPointer", "");
								return x;
							}
						}
						RawPointerMapLog(pankey_Log_EndMethod, "getValueIndexByPointer", "");
						return -1;
					}

				protected:

					virtual void incrementIndex(){
						RawPointerMapLog(pankey_Log_StartMethod, "incrementIndex", this->m_index);
						this->m_index++;
						RawPointerMapLog(pankey_Log_EndMethod, "incrementIndex", this->m_index);
					}
					virtual void decrementIndex(){
						RawPointerMapLog(pankey_Log_StartMethod, "decrementIndex", this->m_index);
						this->m_index--;
						if(this->m_index < 0){
							this->m_index = 0;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "decrementIndex", this->m_index);
					}

					virtual void incrementIndex(int a_size){
						RawPointerMapLog(pankey_Log_StartMethod, "incrementIndex", this->m_index);
						this->m_index += a_size;
						RawPointerMapLog(pankey_Log_EndMethod, "incrementIndex", this->m_index);
					}
					virtual void decrementIndex(int a_size){
						RawPointerMapLog(pankey_Log_StartMethod, "decrementIndex", this->m_index);
						this->m_index -= a_size;
						if(this->m_index < 0){
							this->m_index = 0;
						}
						RawPointerMapLog(pankey_Log_EndMethod, "decrementIndex", this->m_index);
					}
					
					virtual bool isEqualKey(K* a_key_1, K* a_key_2){
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
					
					virtual bool isEqualValue(V* a_value_1, V* a_value_2){
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
					virtual void destroyKeyPointer(K* a_pointer)=0;
					virtual void destroyValuePointer(V* a_pointer)=0;

					bool m_key_owner = true;
					bool m_value_owner = true;
					int m_index = 0;
					int m_size = 0;
			};

		}

	}

}
