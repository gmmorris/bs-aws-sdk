open Jest;

open MockAws;
let rewiredModule = S3_upload_spec.rewire();

describe("S3", () =>
  testAsync("uploadWithOptions", assertThat => {
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
    S3_upload.mockAwsSdk(
      rewiredModule,
      AwsSdk.(
        aws(~_S3=() =>
          s3Instance(~upload=options =>
            assertThat(Expect.(expect(options) |> toEqual(expected)))
          )
        )
      ),
    ) @@
    (() => rewiredModule |> S3_upload.Rewiring.Tests.uploadWithOptions);
  })
);