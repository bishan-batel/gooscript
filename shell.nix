with import <nixpkgs> { };

clangStdenv.mkDerivation {
  name = "dev-shell";

  src = null;

  nativeBuildInputs = [ gcc clang ninja python39 cmake ];

  permittedInsecurePackages = [  ];

  buildInputs = [ gcc clang ];

  # shellHook = ''
  #    # code ran when entering shell
  # '';
}

