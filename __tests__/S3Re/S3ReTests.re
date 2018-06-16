open Jest;

describe("S3", () => {
  testPromise("Can be instanciated", () => {
    module AwsSdk = {
      type s3 = unit => unit;
      [@bs.deriving abstract]
      type aws = {_S3: s3};
    };
    S3ReExecutors.mockAwsSdkAndTestAsync(
      ~setup=capture => AwsSdk.(aws(~_S3=() => capture("instance"))),
      ~execute=
        rewiredModule => S3ReExecutors.Tests.instanciateS3(rewiredModule),
      ~ensure=actual => Expect.(expect(actual) |> toEqual("instance")),
      (),
    );
  });
  testPromise("can upload to S3 With Options", () => {
    module AwsSdk = {
      type upload = Js.t(string) => unit;
      [@bs.deriving abstract]
      type s3Instance = {upload};
      type s3 = unit => s3Instance;
      [@bs.deriving abstract]
      type aws = {_S3: s3};
    };
    S3ReExecutors.mockAwsSdkAndTestAsync(
      ~setup=capture => AwsSdk.(aws(~_S3=() => s3Instance(~upload=capture))),
      ~execute=
        rewiredModule => S3ReExecutors.Tests.uploadWithOptions(rewiredModule),
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
    );
  });
  testPromise("can upload to S3 With Options and a callback", () => {
    module AwsSdk = {
      type upload = (Js.t(string), unit => unit) => unit;
      [@bs.deriving abstract]
      type s3Instance = {
        [@bs.optional]
        upload,
      };
      type s3 = unit => s3Instance;
      [@bs.deriving abstract]
      type aws = {_S3: s3};
    };
    S3ReExecutors.mockAwsSdkAndTestAsync(
      ~setup=
        capture =>
          AwsSdk.(
            aws(~_S3=() =>
              s3Instance(~upload=(_, callback) => capture(callback), ())
            )
          ),
      ~execute=
        rewiredModule =>
          S3ReExecutors.Tests.uploadWithOptionsAndCallback(rewiredModule),
      ~ensure=actual => ExpectJs.(expect(actual) |> toThrowMessage("OMG")),
      (),
    );
  });
});