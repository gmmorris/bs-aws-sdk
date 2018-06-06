exception InvalidTestFile;

let rewiredModule = () =>
  switch ([%bs.node __filename]) {
  | Some(modulePath) => MockAws.Rewiring.rewire(modulePath)
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