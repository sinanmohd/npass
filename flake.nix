{
  description = "A passwordstore and Secret Service API implementation";

  inputs.nixpkgs.url = "github:NixOs/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }: let
    lib = nixpkgs.lib;

    supportedSystems = lib.platforms.unix;
    forSystem = f: system: f {
      inherit system;
      pkgs = import nixpkgs { inherit system; };
    };
    forAllSystems = f: lib.genAttrs supportedSystems (forSystem f);
  in {
    devShells = forAllSystems ({ system, pkgs, ... }: {
      default = pkgs.mkShell {
        name = "dev";

        buildInputs = with pkgs; [ gpgme ];
	shellHook = ''
          [ -z "$XDG_DATA_HOME" ] &&
              export XDG_DATA_HOME="$HOME/.local/share"
          export PASSWORD_STORE_DIR="$XDG_DATA_HOME/debug_pass"
          export PS1="\033[0;32m[󰟵 ]\033[0m $PS1"
        '';
      };
    });
  };
}
