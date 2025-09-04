{
  description = "A Nix flake for Arduino development";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            arduino
            arduino-cli
          ];

          shellHook = ''
            export FQBN="arduino:avr:nano"
            export PORT="/dev/ttyUSB0"
            export BAUD=115200
            export FILE="./main/main.ino"
          '';
        };
      }
    );
}
