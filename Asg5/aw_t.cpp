#include "aw_t.h"
#include <algorithm>
aw_t::aw_t()
{
	// C O N S T R U C T O R 
}

bool aw_t::fequal(float f1, float f2)
{
    if(fabs(f1-f2)<0.0001) return true;
    else return false;
}

// load the obj file. read data by lineheader
// -----------------------------------------
void aw_t::loadObj(const char *filename)
{
    // try to open the file, exit if failed
    // ------------------------------------
    std::ifstream ifs(filename, std::ios::in);
    if (!ifs)
    {
	std::cerr << "Cannot open file "<< filename << std::endl;
	exit(1);
    }

    // start read file
    // ---------------
    std::string line;
    while(std::getline(ifs, line))
    {
        // the line header could be {v, vt, vn, g, f, #}. I plan to omit  '#'.
	// ------------------------------------------------------------------
	
	// read vertices coordinates to @param vector<glm::vec3> verts
	// -----------------------------------------------------------
	if (line.substr(0,2)=="v ")
        {
	    std::istringstream v(line.substr(2));
	    glm::vec3 vert;
	    float x,y,z;
	    v>>x;v>>y;v>>z;
	    vert=glm::vec3(x,y,z);
	    verts.push_back(vert);
	}
	
	// read texture coordinates to @param vector<glm::vec2> vt;
	// -------------------------------------------------------
	else if (line.substr(0,2)=="vt")
	{
	    std::istringstream v(line.substr(3));
	    glm::vec2 text;
	    float U, V;
	    v>>U;v>>V;
	    text=glm::vec2(U,V);
	    vt.push_back(text);
	}

	// read normal coordiantes to @param vector<glm::vec3> vn;
	// ------------------------------------------------------
	else if (line.substr(0,2)=="vn")
	{
	    std::istringstream v(line.substr(3));
	    glm::vec3 norm;
   	    float nx,ny,nz;
	    v>>nx;v>>ny;v>>nz;
	    norm=glm::vec3(nx,ny,nz);
	    vn.push_back(norm);
	}
	
	// read group name to @param string group
	// -------------------------------------
	else if (line.substr(0,2)=="g ")
	{
	    std::istringstream v(line.substr(2));
	    v>>group;
	}

	// read face index to @param vi
	// read texture index to @param vti     if existed
	// read norm index to @param vni        if existed
	// -----------------------------------------------
	else if (line.substr(0,2)=="f ")
	{
	    // verify the format
	    // -----------------  
	    int nslash=std::count(line.begin(),line.end(),'/');
	    // f v v v
	    if(nslash==0)
	    {
	        std::istringstream v(line.substr(2));
		int vix,viy,viz;
		// notice the index begin with 0
		v>>vix;vi.push_back(--vix);
                v>>viy;vi.push_back(--viy);
		v>>viz;vi.push_back(--viz);
            }
	    // f v/vt v/vt v/vt
	    else if(nslash==3)
	    {
		int vix,viy,viz,vtix,vtiy,vtiz;
		const char* ch=line.c_str();
		sscanf(ch, "f %d/%d %d/%d %d/%d", &vix,&vtix,&viy,&vtiy,&viz,&vtiz);
		vi.push_back(--vix);
		vi.push_back(--viy);
		vi.push_back(--viz);
		vti.push_back(--vtix);
		vti.push_back(--vtiy);
		vti.push_back(--vtiz);
            }
	    else if (nslash==6)
	    {	
		int pos=line.find('/');
		// f v//vn v//vn v//vn
		if(line[pos+1]=='/')
		{
		    int vix,viy,viz,vnix,vniy,vniz;
		    const char* ch=line.c_str();
		    sscanf(ch, "f %d//%d %d//%d %d//%d", &vix,&vnix,&viy,&vniy,&viz,&vniz);
		    vi.push_back(--vix);
	  	    vi.push_back(--viy);
		    vi.push_back(--viz);
		    vni.push_back(--vnix);
		    vni.push_back(--vniy);
		    vni.push_back(--vniz);
		}
		// f v/vt/vn v/vt/vn v/vt/vn
		else
		{
		    int vix,viy,viz,vtix,vtiy,vtiz,vnix,vniy,vniz;
		    const char* ch=line.c_str();
		    sscanf(ch, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vix,&vtix,&vnix,&viy,&vtiy,&vniy,&viz,&vtiz,&vniz);
	            vi.push_back(--vix);
	  	    vi.push_back(--viy);
		    vi.push_back(--viz);
                    vti.push_back(--vtix);
		    vti.push_back(--vtiy);
		    vti.push_back(--vtiz);
		    vni.push_back(--vnix);
		    vni.push_back(--vniy);
		    vni.push_back(--vniz);
		}	
            }
	}
    }
}

// return index of verts for EBO
// ----------------------------
std::vector<unsigned int> aw_t::getVi()
{
    if(vi.empty())
    {
	std::cerr<<" vi is empty, please check "<<std::endl;
	exit(1);
    }
    else
    {
	return vi;
    }
}

// return group name
// -----------------
std::string aw_t::getGroup()
{
    if(!group.empty())
    {
	return group;
    }
    else
    {
	std::cout<<" no group name "<<std::endl;
    }
}

// return vertices coordiantes
// --------------------------
std::vector<glm::vec3> aw_t::getVerts()
{
    if(verts.empty())
    {
        std::cerr<<" verts is empty, please check "<<std::endl;
	exit(1);
    }
    else
    {
	return verts;
    }
}

// check if the obj file contain texture coordiante
// -----------------------------------------------
bool aw_t::isVt()
{
    return !vt.empty();
}

// check if the obj file contain normal coordiante
// -----------------------------------------------
bool aw_t::isVn()
{
    return !vn.empty();
}


// assemble the data to one GLfloat array for VBO
// ----------------------------------------------
GLfloat* aw_t::pack(bool calc_normal)
{   
    if(verts.empty())
    {
	std::cerr<< " load the file first then call pack()" << std::endl;
	exit(1);
    }
    GLfloat *data;
    if(isVt() && isVn())
    {
        VERTEX_LENGTH = 8;
	dataSize=8*vi.size();
        data = new  GLfloat[dataSize];
    }
    else if(isVt())
    {	
	if (calc_normal=true)
	{
	    calc_normals();
	    VERTEX_LENGTH = 8;
	    dataSize=8*vi.size();
	    data=new GLfloat[dataSize];
	    for(unsigned int i=0;i<vi.size();i++)
	    {  
		data[8*i]=verts[vi[i]].x;
	        data[8*i+1]=verts[vi[i]].y;
	        data[8*i+2]=verts[vi[i]].z;
	        data[8*i+3]=vt[vti[i]].x;
	        data[8*i+4]=vt[vti[i]].y;
		data[8*i+5]=vn[vni[i]].x;
		data[8*i+6]=vn[vni[i]].y;
		data[8*i+7]=vn[vni[i]].z; 
	    } 
	}
	else
	{
	    VERTEX_LENGTH = 5;
	    dataSize=5*vi.size();
            data= new GLfloat[dataSize];
	    for(unsigned int i=0;i<vi.size();i++)
	    {
	        data[5*i]=verts[vi[i]].x;
	        data[5*i+1]=verts[vi[i]].y;
	        data[5*i+2]=verts[vi[i]].z;
	        data[5*i+3]=vt[vti[i]].x;
	        data[5*i+4]=vt[vti[i]].y;
	    }
	}
    }
    else if(isVn())
    {
	VERTEX_LENGTH = 6;
	dataSize=6*vi.size();
	data= new GLfloat[dataSize];

    }
    else
    {
	if (calc_normal=true)
	{
	    calc_normals();
	    VERTEX_LENGTH = 6;
	    dataSize=6*vi.size();
	    data=new GLfloat[dataSize];
	    for(unsigned int i=0;i<vi.size();i++)
	    {
	        data[6*i]=verts[vi[i]].x;
	        data[6*i+1]=verts[vi[i]].y;
	        data[6*i+2]=verts[vi[i]].z;
		data[6*i+3]=vn[vni[i]].x;
		data[6*i+4]=vn[vni[i]].y;
		data[6*i+5]=vn[vni[i]].z; 
	    } 
	}
	else
	{
	    VERTEX_LENGTH = 3;
	    dataSize=3*vi.size();
            data=new GLfloat[dataSize];
	    for(unsigned int i=0;i<vi.size();i++)
	    {
	        data[3*i]=verts[vi[i]].x;
	        data[3*i+1]=verts[vi[i]].y;
	        data[3*i+2]=verts[vi[i]].z; 
	    }
	}  
    }
    return data;
}

// compute the center of geometry
// -----------------------------
GLfloat* aw_t::getCenter()
{   
    if(verts.empty())
    {
	std::cerr<< " load the file first then call pack()" << std::endl;
	exit(1);
    }

    GLfloat *center;
    center = new GLfloat[3];
    unsigned int N= verts.size();
    for(unsigned int i=0;i<N;i++)
    {
	center[0] += verts[i].x/N;
	center[1] += verts[i].y/N;
	center[2] += verts[i].z/N;
    }
    return center;
}

// return VERTEX_LENGTH IN VBO
// ---------------------------
GLuint aw_t::getVERTEXLength()
{
    if(VERTEX_LENGTH == 0)
    {
	return false;
    }
    else
    {
        return VERTEX_LENGTH;
    }
}

// return GLfloat array size
GLuint aw_t::getDataSize()
{
    if(dataSize == 0)
    {
	return false;
    }
    else
    {
	return dataSize;
    }
}
void aw_t::calc_normals()
{
    // initialize size to store face index in position	(vertex was used by which face)
    fi.resize(verts.size());

    glm::vec3 p0, p1, p2, v1, v2;
    glm::vec3 fn;

    std::cerr << "vi.size() = " << vi.size() << std::endl;
    std::cerr << "vi.size()/3 = " << vi.size()/3 << std::endl;

    // assume each face is a triangle (hence iterate by 3)
    for (int f = 0;f <(size_t) vi.size()/3; f++)
    {
	std::cerr << "f" << vi[3*f + 0] + 1 << " ";
	std::cerr <<        vi[3*f + 1] + 1 << " ";
	std::cerr <<        vi[3*f + 2] + 1 << std::endl;

	// for each vertex, add in this face index
	fi[vi[3*f + 0]].push_back(f);
	fi[vi[3*f + 1]].push_back(f);
	fi[vi[3*f + 2]].push_back(f);

	p0 = verts[vi[3*f + 0]];
	p1 = verts[vi[3*f + 1]];
	p2 = verts[vi[3*f + 2]];

	v1 = p1 - p0;
	v2 = p2 - p0;

	// face normal
	fn = normalize(cross(v1, v2));
	
	std::cerr << glm::to_string(fn) << std::endl;
	
	fnrms.push_back(fn);
    } 

    // fi[v] is the list of face indices incident on vertex v

    // debug: print out all fi (face indices) incident on each vertex
    for (int v=0; v<(size_t) fi.size();v++) 
    {
	for (int f=0; f<(size_t) fi[v].size(); f++)
	{
	    std::cerr << fi[v][f] + 1 << " ";
	}
	std::cerr << std::endl;
    }

    // compute vertex normals as average of face normals incident on vertex
    unsigned int n = 0;
    glm::vec3 vfn;
    if (vni.empty())
    {
    	//vni.resize(vi.size());
	vni = vi;
   	for (int v=0; v<(size_t) fi.size();v++)
	{
	    vfn = glm::vec3(0.0f, 0.0f, 0.0f);
	    std::vector<glm::vec3> used;
	    for (int f=0; f<(size_t)fi[v].size();f++)
	    {
		bool duplication=false;
		for (int i=0; i < used.size();i++)
		{
		    if(fequal(fnrms[fi[v][f]].x,used[i].x)&&fequal(fnrms[fi[v][f]].y,used[i].y)&&fequal(fnrms[fi[v][f]].z,used[i].z))
		    {
			duplication = true;
			break;
		    }
		}
		if(!duplication) used.push_back(fnrms[fi[v][f]]);
		else continue;
		vfn +=  fnrms[fi[v][f]];
	    }
	    //vfn = (float)(1.0/(float)count*vfn;
	    vfn = normalize(vfn);
	    vn.push_back(vfn);
	    // vni.push_back(n);
	    //vni[v] = n;
	    //n++;
	}
    }
    
    // debug: print out vertex normals
    for(int v=0;v<(size_t)vn.size();v++)
    {
	std::cerr << glm::to_string(vn[v]) << std::endl;
    }
}
