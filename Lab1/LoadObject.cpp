#include "LoadObject.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareObjectIndexPtr(const ObjectIndex* a, const ObjectIndex* b);
static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseObjectIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseObjectFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

ObjectModel::ObjectModel(const std::string& fileName)
{
	trueUVs = false;
	trueNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->_uvs.push_back(ParseObjectVector2(line));
                    else if(lineCStr[1] == 'n')
                        this->_normals.push_back(ParseObjectVector3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->_vertices.push_back(ParseObjectVector3(line));
                break;
                case 'f':
                    CreateObjectFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void IndexedModel::CalcNormals()
{
    for(unsigned int i = 0; i < _indices.size(); i += 3)
    {
        int i0 = _indices[i];
        int i1 = _indices[i + 1];
        int i2 = _indices[i + 2];

        glm::vec3 v1 = _positions[i1] - _positions[i0];
        glm::vec3 v2 = _positions[i2] - _positions[i0];
        
        glm::vec3 _normal = glm::normalize(glm::cross(v1, v2));
            
        _normals[i0] += _normal;
        _normals[i1] += _normal;
        _normals[i2] += _normal;
    }
    
    for(unsigned int i = 0; i < _positions.size(); i++)
        _normals[i] = glm::normalize(_normals[i]);
}

IndexedModel ObjectModel::ToIndexedModel()
{
    IndexedModel _result;
    IndexedModel _normalModel;
    
    unsigned int numIndices = ObjectIndices.size();
    
    std::vector<ObjectIndex*> indexLookup;
    
    for(unsigned int i = 0; i < numIndices; i++)
        indexLookup.push_back(&ObjectIndices[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareObjectIndexPtr);
    
    std::map<ObjectIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        ObjectIndex* currentIndex = &ObjectIndices[i];
        
        glm::vec3 currentPosition = _vertices[currentIndex->vertex_Index];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;
        
        if(trueUVs)
            currentTexCoord = _uvs[currentIndex->uv_Index];
        else
            currentTexCoord = glm::vec2(0,0);
            
        if(trueNormals)
            currentNormal = _normals[currentIndex->norm_Index];
        else
            currentNormal = glm::vec3(0,0,0);
        
        unsigned int normal_ModelIndex;
        unsigned int result_ModelIndex;
        
        //Create model to properly generate normals on
        std::map<ObjectIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normal_ModelIndex = _normalModel._positions.size();
        
            normalModelIndexMap.insert(std::pair<ObjectIndex, unsigned int>(*currentIndex, normal_ModelIndex));
            _normalModel._positions.push_back(currentPosition);
            _normalModel._textureCoords.push_back(currentTexCoord);
            _normalModel._normals.push_back(currentNormal);
        }
        else
            normal_ModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindingLastVertexIndex(indexLookup, currentIndex, _result);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            result_ModelIndex = _result._positions.size();
        
            _result._positions.push_back(currentPosition);
            _result._textureCoords.push_back(currentTexCoord);
            _result._normals.push_back(currentNormal);
        }
        else
            result_ModelIndex = previousVertexLocation;
        
        _normalModel._indices.push_back(normal_ModelIndex);
        _result._indices.push_back(result_ModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(result_ModelIndex, normal_ModelIndex));
    }
    
    if(!trueNormals)
    {
        _normalModel.CalcNormals();
        
        for(unsigned int i = 0; i < _result._positions.size(); i++)
            _result._normals[i] = _normalModel._normals[indexMap[i]];
    }
    
    return _result;
};

unsigned int ObjectModel::FindingLastVertexIndex(const std::vector<ObjectIndex*>& indexLookup, const ObjectIndex* currentIndex, const IndexedModel& result)
{
    unsigned int start = 0;
    unsigned int end = indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        ObjectIndex* test_Index = indexLookup[current];
        
        if(test_Index->vertex_Index == currentIndex->vertex_Index)
        {
            unsigned int countStart = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                ObjectIndex* possIndex = indexLookup[current - i];
                
                if(possIndex == currentIndex)
                    continue;
                    
                if(possIndex->vertex_Index != currentIndex->vertex_Index)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                ObjectIndex* possIndex = indexLookup[current + i];
                
                if(possIndex == currentIndex)
                    continue;
                    
                if(possIndex->vertex_Index != currentIndex->vertex_Index)
                    break;
                else if((!trueUVs || possIndex->uv_Index == currentIndex->uv_Index) 
                    && (!trueNormals || possIndex->norm_Index == currentIndex->norm_Index))
                {
                    glm::vec3 currentPosition = _vertices[currentIndex->vertex_Index];
                    glm::vec2 currentTextureCoord;
                    glm::vec3 currentNormal;
                    
                    if(trueUVs)
                        currentTextureCoord = _uvs[currentIndex->uv_Index];
                    else
                        currentTextureCoord = glm::vec2(0,0);
                        
                    if(trueNormals)
                        currentNormal = _normals[currentIndex->norm_Index];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < result._positions.size(); j++)
                    {
                        if(currentPosition == result._positions[j] 
                            && ((!trueUVs || currentTextureCoord == result._textureCoords[j])
                            && (!trueNormals || currentNormal == result._normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(test_Index->vertex_Index < currentIndex->vertex_Index)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void ObjectModel::CreateObjectFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->ObjectIndices.push_back(ParseObjectIndex(tokens[1], &this->trueUVs, &this->trueNormals));
    this->ObjectIndices.push_back(ParseObjectIndex(tokens[2], &this->trueUVs, &this->trueNormals));
    this->ObjectIndices.push_back(ParseObjectIndex(tokens[3], &this->trueUVs, &this->trueNormals));

    if((int)tokens.size() > 4)
    {
        this->ObjectIndices.push_back(ParseObjectIndex(tokens[1], &this->trueUVs, &this->trueNormals));
        this->ObjectIndices.push_back(ParseObjectIndex(tokens[3], &this->trueUVs, &this->trueNormals));
        this->ObjectIndices.push_back(ParseObjectIndex(tokens[4], &this->trueUVs, &this->trueNormals));
    }
}

ObjectIndex ObjectModel::ParseObjectIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    unsigned int tokenLength = token.length();
    const char* tokenString = token.c_str();
    
    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, '/');
    
    ObjectIndex result;
    result.vertex_Index = ParseObjectIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uv_Index = 0;
    result.norm_Index = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uv_Index = ParseObjectIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, '/');
    
    result.norm_Index = ParseObjectIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

glm::vec3 ObjectModel::ParseObjectVector3(const std::string& line) 
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec3(x,y,z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 ObjectModel::ParseObjectVector2(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareObjectIndexPtr(const ObjectIndex* a, const ObjectIndex* b)
{
    return a->vertex_Index < b->vertex_Index;
}

static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline unsigned int ParseObjectIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseObjectFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}