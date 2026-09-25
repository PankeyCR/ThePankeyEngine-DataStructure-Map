#pragma once

#include "RawPointerMap.hpp"

#if defined(pankey_Log) && (defined(ArrayRawPointerMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define ArrayRawPointerMapLog(status,method,mns) pankey_Log(status,"ArrayRawPointerMap",method,mns)
#else
	#define ArrayRawPointerMapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class K,class V>
			class ArrayRawPointerMap : virtual public RawPointerMap<K,V>{
				public:

					ArrayRawPointerMap(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawPointerMap(const ArrayRawPointerMap<K,V>& c_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Copy Constructor", "");

						for(int x = 0; x < c_map.length(); x++){
							this->m_keys[x] = c_map.m_keys[x];
							this->m_values[x] = c_map.m_values[x];
							this->attachKeyPointer(this->m_keys[x]);
							this->attachValuePointer(this->m_values[x]);
						}

						this->m_index = c_map.m_index;
						this->m_size = c_map.m_size;
						
						this->m_reorder = c_map.m_reorder;
						this->m_expandSize = c_map.m_expandSize;

						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Copy Constructor", "");
					}

					ArrayRawPointerMap(ArrayRawPointerMap<K,V>&& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");

						this->m_index = a_map.m_index;
						this->m_size = a_map.m_size;
						
						this->m_reorder = a_map.m_reorder;
						this->m_expandSize = a_map.m_expandSize;

						this->m_keys = a_map.m_keys;
						this->m_values = a_map.m_values;

						a_map.m_index = 0;
						a_map.m_size = 0;

						a_map.m_keys = nullptr;
						a_map.m_values = nullptr;
						
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					virtual ~ArrayRawPointerMap(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Destructor", "");
						if(this->m_keys != nullptr && this->m_values != nullptr){
							ArrayRawPointerMapLog(pankey_Log_Statement, "Destructor", "this->m_keys != nullptr && this->m_values != nullptr");
							for(int x = 0; x < this->length(); x++){
								K* f_key = this->m_keys[x];
								V* f_value = this->m_values[x];

								this->releaseKeyPointer(f_key);
								this->releaseValuePointer(f_value);

								this->destroyKeyPointer(f_key);
								this->destroyValuePointer(f_value);
							}
							ArrayRawPointerMapLog(pankey_Log_StartMethod, "Destructor", "after deleting");
							this->m_index = 0;
							this->m_size = 0;
							this->destroyKeyPointerArray(this->m_keys);
							this->destroyValuePointerArray(this->m_values);
							this->m_keys = nullptr;
							this->m_values = nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Destructor", "");
					}

					virtual bool isEmpty()const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "isEmpty", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "isEmpty", "");
						return this->length() <= 0 || this->m_keys == nullptr || this->m_values == nullptr;
					}

					virtual bool addPointers(K* a_key, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "addPointers", "");
						if(this->length() >= this->getSize()){
							ArrayRawPointerMapLog(pankey_Log_Statement, "addPointers", "this->length() >= this->getSize()");
							return false;
						}
						this->m_keys[this->length()] = a_key;
						this->m_values[this->length()] = a_value;
						this->incrementIndex();

						this->attachKeyPointer(a_key);
						this->attachValuePointer(a_value);

						ArrayRawPointerMapLog(pankey_Log_EndMethod, "addPointers", "");
						return true;
					}

					virtual bool addFastPointers(K* a_key, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "addFastPointers", "");
						
						this->m_keys[this->length()] = a_key;
						this->m_values[this->length()] = a_value;
						this->incrementIndex();

						this->attachKeyPointer(a_key);
						this->attachValuePointer(a_value);

						ArrayRawPointerMapLog(pankey_Log_EndMethod, "addFastPointers", "");
						return true;
					}

					virtual bool setKeyPointerByIndex(int a_index, K* a_key){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setKeyPointerByIndex", "");
						if(!this->hasAvailableSize(a_index) || a_key == nullptr){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return false;
						}
						K* i_key = this->getKeyPointerByIndex(a_index);
						if(a_key == i_key){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return true;
						}
						
						this->m_keys[a_index] = a_key;

						this->releaseKeyPointer(i_key);
						this->attachKeyPointer(a_key);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
						return true;
					}

					virtual bool setValuePointerByIndex(int a_index, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setValuePointerByIndex", "");
						if(!this->hasAvailableSize(a_index)){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return false;
						}
						V* i_value = this->getValuePointerByIndex(a_index);
						if(a_value == i_value){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return true;
						}
						
						this->m_values[a_index] = a_value;

						this->releaseValuePointer(i_value);
						this->attachValuePointer(a_value);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
						return true;
					}

					virtual V* getValuePointerByIndex(int a_index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getValuePointerByIndex", "");
						if(a_index >= this->length() || this->isEmpty()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByIndex", "");
							return nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByIndex", "");
						return this->m_values[a_index];
					}

					virtual V* getFastValuePointerByIndex(int a_index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getFastValuePointerByIndex", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getFastValuePointerByIndex", "");
						return this->m_values[a_index];
					}

					virtual K* getKeyPointerByIndex(int a_index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getKeyPointerByIndex", "");
						if(a_index >= this->length() || this->isEmpty()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByIndex", "");
							return nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByIndex", "");
						return this->m_keys[a_index];
					}

					virtual K* getFastKeyPointerByIndex(int a_index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getFastKeyPointerByIndex", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getFastKeyPointerByIndex", "");
						return this->m_keys[a_index];
					}

					virtual void reset(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "reset", "");
						for(int x = 0; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}
						this->m_index = 0;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "reset", "");
					}

					virtual void clear(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clear", "");
						ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "this->length()");
						ArrayRawPointerMapLog(pankey_Log_Statement, "clear", this->length());
						for(int x = 0; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->destroyKeyPointer(this->m_keys[x]);
							this->destroyValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}
						this->m_index = 0;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clear", "");
					}

					virtual void clearValue(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clearValue", "");
						for(int x = 0; x < this->length(); x++){
							this->releaseValuePointer(this->m_values[x]);
							this->destroyValuePointer(this->m_values[x]);
							this->m_values[x] = nullptr;
						}
						this->m_index = 0;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clearValue", "");
					}

					virtual bool removePointersByIndex(int a_index){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "removePointersByIndex", "");
						if(a_index >= this->length() || this->isEmpty() || a_index < 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
							return false;
						}
						K* i_key = this->m_keys[a_index];
						V* i_value = this->m_values[a_index];
						this->m_keys[a_index] = nullptr;
						this->m_values[a_index] = nullptr;
						int i_iteration = this->length();
						this->decrementIndex();

						this->releaseKeyPointer(i_key);
						this->releaseValuePointer(i_value);

						if(!this->m_reorder){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
							return true;
						}
						for(int x = a_index + 1; x < i_iteration; x++){
							this->m_keys[x - 1] = this->m_keys[x];
							this->m_values[x - 1] = this->m_values[x];
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
						return true;
					}

					virtual bool shrinkLocalSize(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "shrinkLocalSize", "");

						if(a_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocalSize", "a_size <= 0");
							return false;
						}

						int i_size = this->getSize() - a_size;

						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocalSize", "Shrinking size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocalSize", i_size);

						if(i_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocalSize", "i_size <= 0");
							return false;
						}

						K** nK = this->createKeyPointerArray(i_size);
						V** nV = this->createValuePointerArray(i_size);

						int i_new_size = this->m_index < i_size ? this->m_index : i_size;

						for(int x=0; x < i_new_size; x++){
							nK[x] = this->m_keys[x];
							nV[x] = this->m_values[x];
						}

						for(int x = i_new_size; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}

						this->destroyKeyPointerArray(this->m_keys);
						this->destroyValuePointerArray(this->m_values);

						this->m_keys = nK;
						this->m_values = nV;

						this->m_size = i_size;
						this->m_index = i_new_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocalSize", "");
						return true;
					}

					virtual bool shrinkLocal(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "shrinkLocal", "");

						if(a_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocal", "a_size <= 0");
							return false;
						}

						int i_size = this->length() - a_size;

						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocal", "Shrinking size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocal", i_size);

						if(i_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocal", "i_size <= 0");
							return false;
						}

						int i_new_size = this->m_index < i_size ? this->m_index : i_size;

						for(int x = i_new_size; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}

						this->m_index = i_new_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocal", "");
						return true;
					}

					//resize length by adding more space
					virtual bool expandLocalSize(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "expandLocalSize", "");

						int i_size = this->length() + a_size;

						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "Expanding size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", i_size);

						if(i_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocalSize", "i_size <= 0");
							return false;
						}

						K** nK = this->createKeyPointerArray(i_size);
						V** nV = this->createValuePointerArray(i_size);
						
						if(this->m_keys != nullptr && this->m_values != nullptr){
							for(int x=0; x < this->getSize(); x++){
								nK[x] = this->m_keys[x];
								nV[x] = this->m_values[x];
							}
						}else if(this->m_keys != nullptr){
							for(int x=0; x < this->getSize(); x++){
								nK[x] = this->m_keys[x];
							}
							delete[] this->m_keys;
							this->m_keys = nullptr;
						}else if(this->m_values != nullptr){
							for(int x=0; x < this->getSize(); x++){
								nV[x] = this->m_values[x];
							}
							delete[] this->m_values;
							this->m_values = nullptr;
						}
						for(int x = this->getSize(); x < i_size; x++){
							nK[x] = nullptr;
							nV[x] = nullptr;
						}
						this->m_keys = nK;
						this->m_values = nV;
						this->m_size = i_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocalSize", "");
						return true;
					}

					virtual ArrayRawPointerMap<K,V>& operator=(const ArrayRawPointerMap<K,V>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator=", "const ArrayRawPointerMap<K,V>&");
						this->clear();
						for(int x = 0; x < a_map.length(); x++){
							K* f_key = a_map.getKeyPointerByIndex(x);
							V* f_value = a_map.getValuePointerByIndex(x);
							this->addPointers(f_key, f_value);
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator=", "");
						return *this;
					}

					virtual ArrayRawPointerMap<K,V>& operator=(ArrayRawPointerMap<K,V>&& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator=", "ArrayRawPointerMap<K,V>&&");

						this->m_key_owner = a_map.m_key_owner;
						this->m_value_owner = a_map.m_value_owner;
						this->m_index = a_map.m_index;
						this->m_size = a_map.m_size;
						
						this->m_destroyKeyPointer = a_map.m_destroyKeyPointer;
						this->m_destroyValuePointer = a_map.m_destroyValuePointer;
						this->m_reorder = a_map.m_reorder;
						this->m_expandSize = a_map.m_expandSize;

						this->m_keys = a_map.m_keys;
						this->m_values = a_map.m_values;
						
						a_map.m_destroyKeyPointer = nullptr;
						a_map.m_destroyValuePointer = nullptr;

						a_map.m_keys = nullptr;
						a_map.m_values = nullptr;
						
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator=", "ArrayRawPointerMap<K,V>&&");
						return *this;
					}

					virtual bool operator==(const ArrayRawPointerMap<K,V>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator==", "const ArrayRawPointerMap<K,V>&");
						if(a_map.length() != this->length()){
							return false;
						}
						for(int x = 0; x < a_map.length(); x++){
							K* f_key_1 = a_map.getKeyPointerByIndex(x);
							V* f_value_1 = a_map.getValuePointerByIndex(x);
							K* f_key_2 = this->getKeyPointerByIndex(x);
							V* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 != f_key_2 || f_value_1 != f_value_2){
								return false;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator==", "");
						return true;
					}

					virtual bool operator!=(const ArrayRawPointerMap<K,V>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator!=", "const ArrayRawPointerMap<K,V>&");
						if(a_map.length() != this->length()){
							return true;
						}
						for(int x = 0; x < a_map.length(); x++){
							K* f_key_1 = a_map.getKeyPointerByIndex(x);
							V* f_value_1 = a_map.getValuePointerByIndex(x);
							K* f_key_2 = this->getKeyPointerByIndex(x);
							V* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 != f_key_2 || f_value_1 != f_value_2){
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator!=", "");
						return false;
					}

				protected:
					
					virtual K** createKeyPointerArray(int a_size){return nullptr;}
					virtual V** createValuePointerArray(int a_size){return nullptr;}
					
					virtual void destroyKeyPointerArray(K** a_pointer){}
					virtual void destroyValuePointerArray(V** a_pointer){}

					bool m_reorder = true;
					int m_expandSize = 5;

					K** m_keys = nullptr;
					V** m_values = nullptr;
			};
		
		}
		
	}

}
