open Jest;

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

module Mock = {
  let uploadWithOptions =
      (rewiredModule: RewiredTestModule.t, onMockSetup: unit => unit) =>
    Js.Promise.make((~resolve, ~reject) => {
      RewiredTestModule.set(
        rewiredModule,
        "AwsSdk",
        MockAws.onUploadToS3(opts => resolve(. opts)),
      )
      |> ignore;
      onMockSetup();
    });
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