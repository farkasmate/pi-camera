require "baked_file_system"

class FileStorage
  extend BakedFileSystem

  bake_folder "resources/"
end
