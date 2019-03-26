#include "aw_t.h"
#include <algorithm>
aw_t::aw_t()
{
	// C O N S T R U C T O R 
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
GLfloat* aw_t::pack()
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
    else if(isVn())
    {
	VERTEX_LENGTH = 6;
	dataSize=6*vi.size();
	data= new GLfloat[dataSize];

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
