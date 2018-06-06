exception InvalidTestFile;

open Rewire;

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

module Rewiring = {
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
};

let mockAwsSdk = (rewiredModule, mockModule) =>
  Rewiring.Tests.withRewiringOver(rewiredModule, "AwsSdk", mockModule);

let rewire = () =>
  switch ([%bs.node __filename]) {
  | Some(modulePath) => Rewiring.rewire(modulePath)
  | None => raise(InvalidTestFile)
  };