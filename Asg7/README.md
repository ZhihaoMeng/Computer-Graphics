## Info

* CPSC6050    Section:01
  
* Asg07

* CHONG MENG

## KEY  <------------->  Camera Action

### Translation:

X-axis:
  __A__ <----------> move right (pos.x is increased)
  **D** <----------> move left (pos.x is decreased)

Y-axis:
	**E** <----------> move down (pos.y is decreased)
  **Q** <----------> move up   (pos.y is increased)

Z-axis:
  **W** <----------> move foward (pos.z is decreased)
  **S** <----------> move backfoward (pos.z is increased)

  ### Rotation

X-axis:
  **H** <----------> rotate clockwise (degree is decreased)
  **K** <----------> rotate counterclockwise (degree is increased)

Y-axis:
  **Y** <----------> rotate clockwise (degree is decreased)
  **I** <----------> rotate counterclockwise (degree is increased)

Z-axis:
  **U** <----------> rotate clockwise (degree is decreased)
  **J** <----------> rotate counterclockwise (degree is increased)

## Description

* *__camera__* class for camera object
* *__aw_t__* class to read .obj file

* __phong.vert__ and __phong.frag__ shaders for lighting based on phong model

* Draw the normals via geometry shader

### the phong lighting model

* fix the normal as N_world in phong.vert

* implement the phong model in phong.frag, including ambient, diffuse and specular components.
	* key steps would be computing the Id and Is
  
### function used for debugging

* face index and norms are printed to check correctness.

### funtion used for assemble data

* *__aw_t.calc_normals()__* is a private function used to claculate vertex normals by vertices.

* *__aw_t.pack(bool calc_normal)__* return the pointer of data that will be bound to VBO, data will contain computed vertex normals if calc_normal=true.
  
  * This data may contains texture coordinate and normal coordinate. We can call funtion *__aw_t.isVn()__* and *__aw_t.isVt()__* to get the detail or just judge it by *__aw_t.getVERTEXLength()__* as explained below,
  
* *__aw_t.getDataSize()__* return the size of data in VBO

  
* *__aw_t.getVERTEXLength()__* return the VERTEX length in VBO for stride and offset setting.
  
  
* *__aw_t.getCenter()__* return the center of object
