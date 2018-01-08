#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct ObjectIndex
{
    unsigned int vertex_Index;
    unsigned int uv_Index;
    unsigned int norm_Index;
    
    bool operator<(const ObjectIndex& r) const { return vertex_Index < r.vertex_Index; }
};

class IndexedModel
{
public:
    std::vector<glm::vec3> _positions;
    std::vector<glm::vec2> _textureCoords;
    std::vector<glm::vec3> _normals;
    std::vector<unsigned int> _indices;
    
    void CalcNormals();
};

class ObjectModel
{
public:
    std::vector<ObjectIndex> ObjectIndices;
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec2> _uvs;
    std::vector<glm::vec3> _normals;
    bool trueUVs;
    bool trueNormals;
    
    ObjectModel(const std::string& fileName);
    
    IndexedModel ToIndexedModel();
private:
    unsigned int FindingLastVertexIndex(const std::vector<ObjectIndex*>& indexLookup, const ObjectIndex* currentIndex, const IndexedModel& result);
    void CreateObjectFace(const std::string& line);
    
    glm::vec2 ParseObjectVector2(const std::string& line);
    glm::vec3 ParseObjectVector3(const std::string& line);
    ObjectIndex ParseObjectIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};

#endif // OBJ_LOADER_H_INCLUDED
