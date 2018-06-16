let instanciateS3 = () => Aws.S3.s3();

let uploadWithOptions = () =>
  Aws.S3.s3()
  |> Aws.S3.upload(
       Aws.S3.uploadParams(
         ~accessControlPolicy=`Private,
         ~body="",
         ~bucket="s3Bucket",
         ~contentType="application/json",
         ~key="key",
         (),
       ),
     );

let uploadWithOptionsAndCallback = () =>
  Aws.S3.s3()
  |> Aws.S3.uploadWithCallback(
       Aws.S3.uploadParams(
         ~accessControlPolicy=`Private,
         ~body="",
         ~bucket="s3Bucket",
         ~contentType="application/json",
         ~key="key",
         (),
       ),
       (_, _) =>
       raise(Invalid_argument("OMG"))
     );

module Tests = {
  include
    Rewire.MakeRewired(
      {
        type t;
      },
    );
  [@bs.send] external uploadWithOptions : t => unit = "";
  [@bs.send] external uploadWithOptionsAndCallback : t => unit = "";
  [@bs.send] external instanciateS3 : t => unit = "";
};

include Rewire.MakeModuleRewiring(Tests);

let mockAwsSdkAndTest = (~setup, ~execute, ~ensure, ()) => {
  let mockableTests = [%bs.node __filename] |> Belt.Option.getExn |> rewire;
  Tests.withRewiringOver(
    mockableTests,
    "AwsSdk",
    setup(result => ensure(result)),
  ) @@
  (() => execute(mockableTests));
};

let mockAwsSdkAndTestAsync = (~setup, ~execute, ~ensure, ()) => {
  let mockableTests = [%bs.node __filename] |> Belt.Option.getExn |> rewire;
  Js.Promise.make((~resolve, ~reject) =>
    Tests.withRewiringOver(
      mockableTests,
      "AwsSdk",
      setup(result => resolve(. result)),
    ) @@
    (() => execute(mockableTests))
  )
  |> Js.Promise.then_(result => Js.Promise.resolve(ensure(result)));
};