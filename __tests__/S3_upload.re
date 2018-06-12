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

module Tests = {
  include
    Rewire.MakeRewired(
      {
        type t;
      },
    );
  [@bs.send] external uploadWithOptions : t => unit = "";
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