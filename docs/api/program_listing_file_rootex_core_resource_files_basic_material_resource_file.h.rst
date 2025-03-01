
.. _program_listing_file_rootex_core_resource_files_basic_material_resource_file.h:

Program Listing for File basic_material_resource_file.h
=======================================================

|exhale_lsh| :ref:`Return to documentation for file <file_rootex_core_resource_files_basic_material_resource_file.h>` (``rootex\core\resource_files\basic_material_resource_file.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "resource_file.h"
   #include "renderer/shader.h"
   #include "material_resource_file.h"
   #include "image_resource_file.h"
   
   class BasicMaterialResourceFile : public MaterialResourceFile
   {
   private:
       static inline Ptr<Shader> s_Shader;
       static inline Microsoft::WRL::ComPtr<ID3D11SamplerState> s_Sampler;
   
       BasicMaterialData m_MaterialData;
   
       Ref<ImageResourceFile> m_DiffuseImageFile;
       Ref<ImageResourceFile> m_NormalImageFile;
       Ref<ImageResourceFile> m_SpecularImageFile;
       Ref<ImageResourceFile> m_LightmapImageFile;
   
   protected:
       Microsoft::WRL::ComPtr<ID3D11Buffer> m_PSCB;
       Microsoft::WRL::ComPtr<ID3D11Buffer> m_VSCB;
   
       BasicMaterialResourceFile(const Type type, const FilePath& path);
   
   public:
       static void Load();
       static void Destroy();
   
       explicit BasicMaterialResourceFile(const FilePath& path);
       virtual ~BasicMaterialResourceFile() = default;
   
       void setColor(const Color& color);
       void setDiffuse(Ref<ImageResourceFile> diffuse);
       void setNormal(Ref<ImageResourceFile> normal);
       void setSpecular(Ref<ImageResourceFile> spec);
       void setLightmap(Ref<ImageResourceFile> lightmap);
       void setAffectedBySky(bool status) { m_MaterialData.pixelBufferData.affectedBySky = status; }
       void setAffectedByLight(bool status) { m_MaterialData.pixelBufferData.isLit = status; }
   
       Color getColor() { return m_MaterialData.pixelBufferData.color; };
       Ref<ImageResourceFile> getDiffuse() { return m_DiffuseImageFile; };
       Ref<ImageResourceFile> getNormal() { return m_NormalImageFile; };
       Ref<ImageResourceFile> getSpecular() { return m_SpecularImageFile; };
       Ref<ImageResourceFile> getLightmap() { return m_LightmapImageFile; };
   
       const Shader* getShader() const override { return s_Shader.get(); };
       Vector<Ref<GPUTexture>> getTextures() const override;
   
       void bindShader() override;
       void bindTextures() override;
       void bindSamplers() override;
       void bindVSCB() override;
       void bindPSCB() override;
   
       JSON::json getJSON() const override;
       ID3D11ShaderResourceView* getPreview() const override;
   
       void reimport() override;
       bool save() override;
       void draw() override;
   };
