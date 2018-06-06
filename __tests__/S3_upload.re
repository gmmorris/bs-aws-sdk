exception InvalidTestFile;

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

module Rewiring = {
  include MakeModuleRewiring(Tests);
};

let mockAwsSdk = (rewiredModule, mockModule) =>
  Tests.withRewiringOver(rewiredModule, "AwsSdk", mockModule);

let rewiredModule = () =>
  switch ([%bs.node __filename]) {
  | Some(modulePath) => Rewiring.rewire(modulePath)
  | None => raise(InvalidTestFile)
  };

let uploadWithOptions = () =>
  Aws.S3.s3()
  |> Aws.S3.upload(
       Aws.S3.uploadOptions(
         ~accessControlPolicy=`Private,
         ~body="",
         ~bucket="s3Bucket",
         ~contentType="application/json",
         ~key="key",
         (),
       ),
     );