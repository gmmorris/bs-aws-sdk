open Rewire;

module Tests = {
  include
    MakeRewired(
      {
        type t;
      },
    );
  [@bs.send] external uploadWithOptions : t => unit = "";
};

include MakeModuleRewiring(Tests);

let mockAwsSdk = (rewiredModule, mockModule) =>
  Tests.withRewiringOver(rewiredModule, "AwsSdk", mockModule);