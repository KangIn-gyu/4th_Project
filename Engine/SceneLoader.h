#pragma once
#include "fstream"
#include "jjson.hpp"
#include "ObjectManager.h"
#include "TransformComponent.h"
using json = nlohmann::json;

static Object::ObjectType fromString(std::string _string) {
    if (_string == "Basic")
        return Object::ObjectType::Basic;
    else if (_string == "Light")
        return Object::ObjectType::Basic;
    else if (_string == "Camera")
        return Object::ObjectType::Basic;
    else if (_string == "UI")
        return Object::ObjectType::Basic;
    else
    {
        std::cout << "json에 태그가 없습니다" << std::endl;
        return Object::ObjectType::Basic;
    }
}

class ObjectData {
public:
    std::string name;
    std::string type; // enumclass 0 = basic 3 = UI 딴것도 쓸지는 모르겠음
    DXMath::Vector3 position;
    DXMath::Quaternion rotation;
    DXMath::Vector3 scale;

    // from_json 함수 정의
    void from_json(const json& j) {
        name = j["objectName"];
        type = j["objectType"];
        position = DXMath::Vector3(j["position"]["x"], j["position"]["y"], j["position"]["z"]);
        rotation = DXMath::Quaternion::CreateFromYawPitchRoll(j["rotation"]["y"], j["rotation"]["x"], j["rotation"]["z"]);
        scale = DXMath::Vector3(j["scale"]["x"], j["scale"]["y"], j["scale"]["z"]);
    }
};

// SceneData 클래스
class SceneData {
public:
    int objNum;
    std::vector<ObjectData> objDatas;

    // from_json 함수 정의
    void from_json(const json& j) {
        objNum = j["objects"].size();
        for (const auto& obj : j["objects"]) {
            ObjectData data;
            data.from_json(obj);
            objDatas.push_back(data);
        }
    }
};

// SceneLoader 클래스
class SceneLoader {
public:
    SceneData scenedata;

    SceneLoader(const std::string& path) {
        std::ifstream inputFile(path);
        if (!inputFile.is_open()) {
            std::cerr << "파일을 열 수 없습니다: " << path << std::endl;
            return;
        }
        else {
            std::cout << "파일이 열렸습니다: " << path << std::endl;
        }

        json jsonData;
        inputFile >> jsonData;
        inputFile.close();

        // SceneData에 JSON 데이터를 넣기
        scenedata.from_json(jsonData);
    }

    void InitObjs(ObjectManager* _managers)
    {
        for (auto& it : scenedata.objDatas)
        {
            auto gameobj = _managers->GetGameObject<Object>(fromString(it.type), it.name);

            DXMath::Matrix translationMatrix = DXMath::Matrix::CreateTranslation(it.position);
            DXMath::Matrix rotationMatrix = DXMath::Matrix::CreateFromQuaternion(it.rotation);
            DXMath::Matrix scaleMatrix = DXMath::Matrix::CreateScale(it.scale);
            auto localMatrix = scaleMatrix * rotationMatrix * translationMatrix;
            gameobj->GetComponent<TransformComponent>()->SetLocalMatrix(localMatrix);
        }
    }
};
