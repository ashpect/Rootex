
.. _program_listing_file_rootex_framework_components_visual_effect_cpu_particles_component.h:

Program Listing for File cpu_particles_component.h
==================================================

|exhale_lsh| :ref:`Return to documentation for file <file_rootex_framework_components_visual_effect_cpu_particles_component.h>` (``rootex\framework\components\visual\effect\cpu_particles_component.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "renderer/vertex_data.h"
   #include "core/resource_files/instancing_basic_material_resource_file.h"
   #include "components/visual/model/model_component.h"
   
   #define MAX_PARTICLES 5000
   
   struct ParticleTemplate
   {
       Vector3 velocity = { 1.0f, 0.0f, 0.0f };
       Color colorBegin = ColorPresets::Red;
       Color colorEnd = ColorPresets::Blue;
       float velocityVariation = 10.0f;
       float rotationVariation = DirectX::XM_PI;
       float angularVelocityVariation = 0.5f;
       float sizeBegin = 0.1f;
       float sizeEnd = 0.0f;
       float sizeVariation = 0.1f;
       float lifeTime = 1.0f;
   };
   
   void to_json(JSON::json& j, const ParticleTemplate p);
   void from_json(const JSON::json& j, ParticleTemplate& p);
   
   class CPUParticlesComponent : public ModelComponent
   {
       COMPONENT(CPUParticlesComponent, Category::Effect);
   
       Vector<InstanceData> m_InstanceBufferData;
       Vector<InstanceData> m_InstanceBufferLiveData;
       int m_LiveParticlesCount;
       Ref<VertexBuffer> m_InstanceBuffer;
   
       struct Particle
       {
           float sizeBegin;
           float sizeEnd;
           float lifeTime;
           float lifeRemaining;
           Color colorBegin;
           Color colorEnd;
           Vector3 velocity;
           Vector3 angularVelocity;
   
           // Not for use outside
           Vector3 position;
           Quaternion rotation;
           Vector3 scale;
       };
   
       ParticleTemplate m_ParticleTemplate;
       Vector<Particle> m_ParticlePool;
       Ref<InstancingBasicMaterialResourceFile> m_ParticlesMaterial;
       size_t m_PoolIndex;
       float m_EmitRate;
   
       enum class EmitMode : int
       {
           Point = 0,
           Square = 1,
           Cube = 2,
           Sphere = 3,
           End
       };
   
       EmitMode m_CurrentEmitMode;
       Vector3 m_EmitterDimensions;
   
       float m_EmitCount = 0;
   
       void expandInstanceData(const size_t& poolSize);
   
   public:
       CPUParticlesComponent(Entity& owner, const JSON::json& data);
       ~CPUParticlesComponent() = default;
   
       void setMaterial(Ref<InstancingBasicMaterialResourceFile> particlesMaterial);
       void emit(const ParticleTemplate& particleTemplate);
       void expandPool(const size_t& poolSize);
   
       bool preRender(float deltaMilliseconds) override;
       void render(float viewDistance) override;
   
       JSON::json getJSON() const override;
       void draw() override;
   };
   
   DECLARE_COMPONENT(CPUParticlesComponent);
