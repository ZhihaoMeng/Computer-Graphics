## Info

* CPSC6050    Section:01
  
* Asg05

* CHONG MENG

## Description

* *__aw_t__* class to read .obj file

* Draw the normals via geometry shader
  
### function used for debugging

* face index and norms are printed to check correctness.

### funtion used for assemble data

* *__aw_t.calc_normals()__* is a private function used to claculate vertex normals by vertices.

* *__aw_t.pack(bool calc_normal)__* return the pointer of data that will be bound to VBO, data will contain computed vertex normals if calc_normal=true.
  
  * This data may contains texture coordinate and normal coordinate. We can call funtion *__aw_t.isVn()__* and *__aw_t.isVt()__* to get the detail or just judge it by *__aw_t.getVERTEXLength()__* as explained below,
  
* *__aw_t.getDataSize()__* return the size of data in VBO

  
* *__aw_t.getVERTEXLength()__* return the VERTEX length in VBO for stride and offset setting.
  
  
* *__aw_t.getCenter()__* return the center of object
