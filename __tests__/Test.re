open Jest;

open MockAws;

describe("S3", () =>
  testPromise("uploadWithOptions", () =>
    S3_upload.mockAwsSdkAndTestAsync(
      ~setup=capture => AwsSdk.(aws(~_S3=() => s3Instance(~upload=capture))),
      ~execute=
        rewiredModule => S3_upload.Tests.uploadWithOptions(rewiredModule),
      ~ensure=
        actual =>
          Expect.(
            expect(actual)
            |> toEqual(
                 [%raw
                   {|
                      {
                        "ACL": "private",
                        "Body": "",
                        "Bucket": "s3Bucket",
                        "ContentType": "application/json",
                        "Key": "key",
                      }
                    |}
                 ],
               )
          ),
      (),
    )
  )
);