open Rewire;

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

module AwsSdk = {
  type upload = string => unit;
  [@bs.deriving abstract]
  type s3Instance = {upload};
  type s3 = unit => s3Instance;
  [@bs.deriving abstract]
  type aws = {_S3: s3};
};

let mockAwsSdk = (rewiredModule, mockModule) =>
  RewiredTestModule.withRewiringOver(rewiredModule, "AwsSdk", mockModule);