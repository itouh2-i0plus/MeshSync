#pragma once

#include "MeshSync/SceneGraph/msMaterial.h"

#ifndef msRuntime
namespace ms {

class MaterialManager
{
public:
    MaterialManager();
    ~MaterialManager();
    bool empty() const;

    // clear all states (both entity and delete record will be cleared)
    void clear();

    // erase entity and add delete record
    bool erase(int id);

    MaterialPtr find(int id) const;

    // thread safe
    // material: valid ID must be assigned (assume generated by ResourceIDGenerator<>)
    bool add(MaterialPtr material);
    bool markDirty(int id);

    std::vector<MaterialPtr> getAllMaterials();
    std::vector<MaterialPtr> getDirtyMaterials();
    std::vector<Identifier>& getDeleted();
    void makeDirtyAll();
    void clearDirtyFlags();

    std::vector<MaterialPtr> getStaleMaterials();
    void eraseStaleMaterials();

    void setAlwaysMarkDirty(bool v);

private:
    struct Record
    {
        MaterialPtr material;
        uint64_t checksum = 0;
        bool dirty = false;
        bool updated = false;
    };
    Record& lockAndGet(int id);

    bool m_always_mark_dirty = false;
    std::map<int, Record> m_records;
    std::vector<Identifier> m_deleted;
    std::mutex m_mutex;
};

} // namespace ms
#endif // msRuntime
