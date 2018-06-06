open Rewire;

module AwsSdk = {
  type upload = string => unit;
  [@bs.deriving abstract]
  type s3Instance = {upload};
  type s3 = unit => s3Instance;
  [@bs.deriving abstract]
  type aws = {_S3: s3};
};

module RewiredTestModule = {
  include
    MakeRewired(
      {
        type t;
      },
    );
  [@bs.send] external uploadWithOptions : t => unit = "";
};

module Rewiring = {
  include MakeModuleRewiring(RewiredTestModule);
};

let asDict = (key, value) => {
  let myMap = Js.Dict.empty();
  Js.Dict.set(myMap, key, value);
  myMap;
};

let mockAwsSdk = (rewiredModule: RewiredTestModule.t, mockModule, onMocked) => {
  let applyMock =
    RewiredTestModule.withRewiring(
      rewiredModule,
      asDict("AwsSdk", mockModule),
    );
  applyMock(onMocked) |> ignore;
};