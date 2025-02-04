#pragma once

#include <vector>
#include <memory>
#include "item.h"
#include "resource/resource.h"
#include "resourceitem.h"
#include "../serialization/serializable.h"

class Game;

class Inventory : public ISerializeable
{
public:
  std::vector<std::shared_ptr<Item>> items;

  Inventory() {}
  Inventory(Serializer &serializer);

  void initAfterLoad(Game &game);

  void add(std::shared_ptr<Item> item);
  void add(int slot, std::shared_ptr<Item> item);
  bool hasResources(Resource::ID resourceId, int count);
  bool removeResource(Resource::ID resourceId, int count);
  bool removeItem(Item &item);
  int count(const Item &item);

  void serialize(Serializer &serializer) override;

private:
  std::shared_ptr<ResourceItem> findResource(Resource::ID resourceId);
};
