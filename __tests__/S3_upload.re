open Jest;

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