open Jest;

open MockAws;

let rewiredModule = S3_upload.rewiredModule();

describe("S3", () =>
  testPromise("uploadWithOptions", () => {
    let expected = [%raw
      {|
        {
          "ACL": "private",
          "Body": "",
          "Bucket": "s3Bucket",
          "ContentType": "application/json",
          "Key": "key",
        }
      |}
    ];
    let onUploadToS3AssertThat = assertThat =>
      mockAwsSdk(
        rewiredModule,
        AwsSdk.(aws(~_S3=() => s3Instance(~upload=assertThat))),
      );
    Js.Promise.make((~resolve, ~reject) =>
      onUploadToS3AssertThat(options =>
        resolve(. Expect.(expect(options) |> toEqual(expected))) |> ignore
      ) @@
      (() => RewiredTestModule.uploadWithOptions(rewiredModule))
    );
  })
);