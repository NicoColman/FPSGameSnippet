import unreal
import json

def load_json_data(file_path):
    with open(file_path, 'r') as file:
        return json.load(file)

def find_weapon_data(weapon_name, data):
    for entry in data:
        if entry["WeaponName"] == weapon_name:
            return entry
    return None

def populate_weapon_data(desired_weapon_name):
    base_asset_path = "/Game/Blueprints/DataAssets/Weapons/Proxies/Firearms/"

    json_data = load_json_data("C:/Users/nicol/OneDrive/Documentos/Unreal Projects/Zombies/scritps/proxy_data.json")

    weapon_data = find_weapon_data(desired_weapon_name, json_data)

    if weapon_data:
        full_asset_path = f"{base_asset_path}DA_W_Proxy_FA_{desired_weapon_name}.DA_W_Proxy_FA_{desired_weapon_name}"

        weapon_data_asset = unreal.load_object(None, full_asset_path)

        if weapon_data_asset:
            weapon_data_asset.set_editor_property("WeaponName", weapon_data["WeaponName"])

            weapon_skeletal_mesh = unreal.Map(unreal.Name, unreal.SkeletalMesh)
            for key, path in weapon_data["WeaponMesh"].items():
                skeletal_mesh = unreal.load_asset(path)
                if skeletal_mesh:
                    weapon_skeletal_mesh[unreal.Name(key)] = skeletal_mesh
                else:
                    print(f"Failed to load skeletal mesh asset: {path}")
            weapon_data_asset.set_editor_property("WeaponMesh", weapon_skeletal_mesh)

            weapon_static_mesh = unreal.Map(unreal.Name, unreal.MeshAttachmentInfo)  # Use your actual struct type here
        
            for key, data in weapon_data["WeaponStaticMeshes"].items():
                socket_name = data["SocketName"]
                mesh_path = data["Mesh"]
            
                static_mesh = unreal.load_asset(mesh_path, unreal.StaticMesh)
                if static_mesh:
                    attachment_info = unreal.MeshAttachmentInfo()  # Create a new instance of your custom struct
                    attachment_info.socket_name = socket_name
                    attachment_info.mesh = static_mesh  # Assign the StaticMesh directly instead of SoftObjectPath
                    weapon_static_mesh[unreal.Name(key)] = attachment_info
                else:
                    print(f"Failed to load static mesh asset: {mesh_path}")
                    
            weapon_data_asset.set_editor_property("WeaponStaticMeshes", weapon_static_mesh)
            
            fp_weapon_data_asset_path = weapon_data["FPWeaponDataAsset"]
            fp_weapon_data_asset = unreal.EditorAssetLibrary.load_asset(fp_weapon_data_asset_path)
            if fp_weapon_data_asset is not None:
                weapon_data_asset.set_editor_property("FPWeaponDataAsset", fp_weapon_data_asset)
                
            tp_weapon_data_asset_path = weapon_data["TPWeaponDataAsset"]
            tp_weapon_data_asset = unreal.EditorAssetLibrary.load_asset(tp_weapon_data_asset_path)
            if fp_weapon_data_asset is not None:
                weapon_data_asset.set_editor_property("TPWeaponDataAsset", tp_weapon_data_asset)

            unreal.EditorAssetLibrary.save_loaded_asset(weapon_data_asset)
            print(f"Weapon data populated successfully for {desired_weapon_name}!")
        else:
            print(f"Failed to load UWeaponProxyDataAsset for {desired_weapon_name}.")
    else:
        print(f"No data found for weapon named {desired_weapon_name}.")

populate_weapon_data("AG14W")
