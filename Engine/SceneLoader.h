#pragma once
#include "fstream"
#include "jjson.hpp"
using json = nlohmann::json;
class ObjectData {
public:
    std::string name;
    DXMath::Vector3 position;
    DXMath::Quaternion rotation;
    DXMath::Vector3 scale;

    // from_json 함수 정의
    void from_json(const json& j) {
        name = j["objectName"];
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
};
