#pragma once

#include "RawPointerMap.hpp"

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class K,class V>
			class RawMapIterator{
				public:
				int position = 0;
				int size = 0;
				RawPointerMap<K,V>* m_map = nullptr;
				
				RawMapIterator(RawPointerMap<K,V>* i_map, int p, int s){
					m_map = i_map;
					position = p;
					size = s;
				}
				
				RawMapIterator(const RawMapIterator<K,V>& entry){
					m_map = entry.m_map;
					position = entry.position;
					size = entry.size;
				}
				
				RawMapIterator(RawMapIterator<K,V>&& entry){
					m_map = entry.m_map;
					position = entry.position;
					size = entry.size;
				}
				
				RawMapIterator& operator=(const RawMapIterator<K,V>& entry){
					m_map = entry.m_map;
					position = entry.position;
					size = entry.size;
					return *this;
				}
				
				RawMapIterator& operator=(RawMapIterator<K,V>&& entry){
					m_map = entry.m_map;
					position = entry.position;
					size = entry.size;
					return *this;
				}
				
				bool operator!=(const RawMapIterator<K,V>& entry){
					return position != entry.size;
				}
				
				bool operator==(const RawMapIterator<K,V>& entry){
					return position == entry.size;
				}
				
				virtual ~RawMapIterator(){
				}
				
				virtual bool isValid(){
					return m_map != nullptr;
				}
				
				virtual RawMapEntry<K,V> get(){
					return m_map->getRawMapEntryByIndex(position);
				}
				
				virtual K getKey(){
					K* key = m_map->getKeyPointerByIndex(position);
					if(key == nullptr){
						return K();
					}
					return *key;
				}
				
				virtual K* getKeyPointer(){
					return m_map->getKeyPointerByIndex(position);
				}
				
				virtual V getValue(){
					V* value = m_map->getValuePointerByIndex(position);
					if(value == nullptr){
						return V();
					}
					return *value;
				}
				
				virtual V* getValuePointer(){
					return m_map->getValuePointerByIndex(position);
				}
				
				virtual void setKey(K k){
					K* key = m_map->getKeyPointerByIndex(position);
					if(key == nullptr){
						return;
					}
					*key = k;
				}
				
				virtual void setValue(V v){
					V* value = m_map->getValuePointerByIndex(position);
					if(value == nullptr){
						return;
					}
					*value = v;
				}
					
				virtual void operator ++(){
					this->position++;
				}
				virtual RawMapIterator<K,V> operator *(){
					return RawMapIterator<K,V>(m_map,position,size);
				}
			};

		}

	}

}