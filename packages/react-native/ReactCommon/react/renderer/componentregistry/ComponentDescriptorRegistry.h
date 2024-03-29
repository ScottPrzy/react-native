/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <memory>
#include <shared_mutex>

#include <butter/map.h>

#include <react/renderer/componentregistry/ComponentDescriptorProvider.h>
#include <react/renderer/core/ComponentDescriptor.h>
#include <react/renderer/core/InstanceHandle.h>
#include <react/utils/ContextContainer.h>

namespace facebook::react {

class ComponentDescriptorProviderRegistry;
class ComponentDescriptorRegistry;

using SharedComponentDescriptorRegistry =
    std::shared_ptr<const ComponentDescriptorRegistry>;

/*
 * Registry of particular `ComponentDescriptor`s.
 */
class ComponentDescriptorRegistry {
 public:
  using Shared = std::shared_ptr<const ComponentDescriptorRegistry>;

  /*
   * Creates an object with stored `ComponentDescriptorParameters`  which will
   * be used later to create `ComponentDescriptor`s.
   */
  ComponentDescriptorRegistry(
      ComponentDescriptorParameters parameters,
      const ComponentDescriptorProviderRegistry &providerRegistry,
      ContextContainer::Shared contextContainer);

  /*
   * This is broken. Please do not use.
   * If you requesting a ComponentDescriptor and unsure that it's there, you are
   * doing something wrong.
   */
  const ComponentDescriptor *
  findComponentDescriptorByHandle_DO_NOT_USE_THIS_IS_BROKEN(
      ComponentHandle componentHandle) const;

  const ComponentDescriptor &at(const std::string &componentName) const;
  const ComponentDescriptor &at(ComponentHandle componentHandle) const;

  bool hasComponentDescriptorAt(ComponentHandle componentHandle) const;

  ShadowNode::Shared createNode(
      Tag tag,
      const std::string &viewName,
      SurfaceId surfaceId,
      const folly::dynamic &props,
      const InstanceHandle::Shared &instanceHandle) const;

  void setFallbackComponentDescriptor(
      const SharedComponentDescriptor &descriptor);
  ComponentDescriptor::Shared getFallbackComponentDescriptor() const;

 private:
  friend class ComponentDescriptorProviderRegistry;

  void registerComponentDescriptor(
      const SharedComponentDescriptor &componentDescriptor) const;

  /*
   * Creates a `ComponentDescriptor` using specified
   * `ComponentDescriptorProvider` and stored `ComponentDescriptorParameters`,
   * and then adds that to the registry.
   * To be used by `ComponentDescriptorProviderRegistry` only.
   * Thread safe.
   */
  void add(ComponentDescriptorProvider componentDescriptorProvider) const;

  mutable std::shared_mutex mutex_;
  mutable butter::map<ComponentHandle, SharedComponentDescriptor>
      _registryByHandle;
  mutable butter::map<std::string, SharedComponentDescriptor> _registryByName;
  ComponentDescriptor::Shared _fallbackComponentDescriptor;
  ComponentDescriptorParameters parameters_{};
  const ComponentDescriptorProviderRegistry &providerRegistry_;
  ContextContainer::Shared contextContainer_;
};

} // namespace facebook::react
